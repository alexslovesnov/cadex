#include "svg.h"

namespace svg {

using namespace std::literals;

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);

    context.out << std::endl;
}

void Document::AddPtr(std::unique_ptr<Object>&& obj){
    objects_.push_back(std::move(obj));
}

void Document::Render(std::ostream& out) const{
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;

    svg::RenderContext ctx(out, 2, 2);
    for(size_t obj_counter = 0; obj_counter < objects_.size(); ++obj_counter){    
        (*objects_.at(obj_counter)).Render(ctx);
    }
    out << "</svg>"sv << std::endl;
}

// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center = Point{0, 0})  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius = 1)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\" "sv;
    out << "/>"sv;
}

// ---------- Polyline ------------------

Polyline& Polyline::AddPoint(Point point){
    points_.push_back(point);
    return *this;
}

void Polyline::RenderObject(const RenderContext& context) const {
    if(points_.size() == 0){return;}
    auto& out = context.out;
    out << "<polyline points=\""sv;
    for(auto it = points_.begin(); it != points_.end(); ++it){
        out << (*it).x << ","sv << (*it).y;
        if(it + 1 != points_.end()) out << " "sv;
    }
    out << "\" />"sv;
}

// ---------- Text ------------------

Text& Text::SetPosition(Point pos) {
    position_ = pos;
    return *this;
}

Text& Text::SetOffset(Point offset) {
    offset_ = offset;
    return *this;
}

Text& Text::SetFontSize(uint32_t size) {
    font_size_ = size;
    return *this;
}

Text& Text::SetFontWeight(std::string font_weight) {
    font_weight_ = font_weight;
    return *this;
}

Text& Text::SetFontFamily(std::string font_family) {
    font_family_ = font_family;
    return *this;
}

Text& Text::SetData(std::string data) {
    data_ = data;
    return *this;
}

std::string Text::StrToSVGText(const std::string& text) {
    std::string result;
    for (char c : text) {
        switch (c) {
            case '"':
                result += "&quot;";
                break;
            case '\'':
                result += "&apos;";
                break;
            case '<':
                result += "&lt;";
                break;
            case '>':
                result += "&gt;";
                break;
            case '&':
                result += "&amp;";
                break;
            default:
                result += c;
                break;
        }
    }
    return result;
}

void Text::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<text";
    out << " x=\"" << position_.x << "\"";
    out << " y=\"" << position_.y << "\"";
    out << " dx=\"" << offset_.x << "\"";
    out << " dy=\"" << offset_.y << "\"";
    out << " font-size=\"" << font_size_ << "\"";
    if (!font_weight_.empty()) {
        out << " font-weight=\"" << font_weight_ << "\"";
    }
    if (!font_family_.empty()) {
        out << " font-family=\"" << font_family_ << "\"";
    }
    out << ">" << data_ << "</text>";
}

}  // namespace svg
