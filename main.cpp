#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;

string num_to_str(double num, int pres){
    string res;
    int f, r;
    double p;
    if(num >= 0){
        f = floor(num);
        p = num - f;
        r = floor(p*pow(10, pres));
    }
    else{
        f = floor(num) + 1;
        p = -num + f;
        r = floor(p*pow(10, pres));
    }
    res = std::to_string(f) + "." + std::to_string(r);
    return(res);
}

class Point {
private:
    double px;
    double py;
    double pz;
 
public:
    Point(double x, double y, double z){
		px = x;
		py = y;
		pz = z;
	}  
 
    //pres - precision
	string Print(int pres){
		return "(" + num_to_str(px, pres) + ", " + num_to_str(py, pres) + ", " + num_to_str(pz, pres) + ")";
	}

};

class Vector {
private:
    double vx;
    double vy;
    double vz;
 
public:
    Vector(double x, double y, double z){
		vx = x;
		vy = y;
		vz = z;
	}  
 
    //pres - precision
	string Print(int pres){
		return "(" + num_to_str(vx, pres) + ", " + num_to_str(vy, pres) + ", " + num_to_str(vz, pres) + ")";
	}

};

class Curve{
public:
	string type;

    Curve(){
		type = "curve";
	}
    
	string GetType(){
		return(type);
	}
	
	virtual Point GetPoint(double t) const {
        return Point(0, 0, 0);
    }
	
	virtual Vector GetTangent(double t) const {
        return Vector(0, 0, 0);
    }
	
};

// parametric planar curve of type r(t) = rad*(cos(t), sin(t))   string Voice() const override
class Circle: public Curve{
private:
	double rad;
	
public:
    Circle(double r){
		rad = r;
		type = "circle";
	}
    
	Point GetPoint(double t) const override{
		double x, y;
		x = rad*cos(t);
		y = rad*sin(t);
		return Point(x, y, 0);
	}
    Vector GetTangent(double t) const override{
		double x, y;
		x = -rad*sin(t);
		y = rad*cos(t);
		return Vector(x, y, 0);
	}	
	
	double GetRad(){
		return(rad);
	}
};

class Ellipse: public Curve{
private:
	double xrad;
	double yrad;
	
public:
    Ellipse(double a, double b){
		xrad = a;
		yrad = b;
		type = "ellipse";
	}
    
	Point GetPoint(double t) const override{
		double x, y;
		x = xrad*cos(t);
		y = yrad*sin(t);
		return Point(x, y, 0);
	}
    Vector GetTangent(double t) const override{
		double x, y;
		x = -xrad*sin(t);
		y = yrad*cos(t);
		return Vector(x, y, 0);
	}
};

class Helix: public Curve{
private:
	double xrad;
    double yrad;
    double step;
	
public:
    Helix(double a, double b, double st){
		xrad = a;
		yrad = b;
		step = st;
		type = "helix";
	}
    
	Point GetPoint(double t) const override{
		double x, y, z;
		x = xrad*cos(t);
		y = yrad*sin(t);
		z = step*t/(2*M_PI);
		return Point(x, y, z);
	}
    Vector GetTangent(double t) const override{
		double x, y, z;
		x = -xrad*sin(t);
		y = yrad*cos(t);
		z = step/(2*M_PI);
		return Vector(x, y, z);
	}
};

bool CircleComp (Circle* a, Circle* b) {
  return (*a).GetRad() < (*b).GetRad();
}


int main(){
    int i, t;
    double a, b, st;
    Curve* cur;
    Circle* circ;
    vector <Curve*> curves;
    vector <Circle*> circles;
    srand(0);
    
    for (int i = 0; i < 20; i++) {
        
        t = rand()%3;
        switch(t){
            case 0:{
                a = rand()%20 + double(rand()%100)/100;
                circ = new Circle(a);
                cur = circ;
                break;
            } 
            case 1:{
                a = rand()%20 + double(rand()%100)/100;
                b = rand()%20 + double(rand()%100)/100;
                cur = new Ellipse(a, b);
                break;
            } 
            case 2:{
                a = rand()%20 + double(rand()%100)/100;
                b = rand()%20 + double(rand()%100)/100;
                st = rand()%20 + double(rand()%100)/100;
                cur = new Helix(a, b, st);
                break;
            } 
        }
        curves.push_back(cur);
        if((*cur).GetType() == "circle"){
            circles.push_back(circ);
        }
    }
    
    //list of poits and derivatives
    /*for (i = 0; i < 20; i++) {
        cout << "point: " << (*curves[i]).GetPoint(M_PI/4).Print(2) << 
        ", derivative: " << (*curves[i]).GetTangent(M_PI/4).Print(2) << "\n";
    }*/
    
    cout << "circles before sorting: \n";
    for (i = 0; i < circles.size(); i++){
        cout << "circle " << i << ": radius = " << (*circles[i]).GetRad() << "\n";
    }
    
    sort(circles.begin(), circles.end(), CircleComp);
    
    cout << "circles after sorting: \n";
    for (i = 0; i < circles.size(); i++){
        cout << "circle " << i << ": radius = " << (*circles[i]).GetRad() << "\n";
    }
    
    cout << "size of curves: " << curves.size() << ", size of circles: " << circles.size() << "\n";
    curves.clear();
    circles.clear();
    return(0);
}
