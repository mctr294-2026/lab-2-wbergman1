#include "roots.hpp" 
#include <iostream>
#include <cmath> 

// 1. Bisection Method
// Algorithm: Repeatedly divides an interval in half and selects the sub-interval 
// where the function changes sign (guaranteeing a root by Intermediate Value Theorem).
bool bisection(std::function<double(double)> f, double a, double b, double *root) {
    // Hardcoded safety limit to prevent infinite loops if it doesn't converge
    for(int i = 0; i < 1e6; i++){

        // Calculate the midpoint of the interval [a, b]
        double midpoint = (a + b)/2;
        double yValue = f(midpoint);

        // Convergence Check: If the function value is close enough to 0, we found the root.
        if (std::abs(yValue) < 1e-6){
            *root = midpoint;
            return true;
        }

        // Determine which sub-interval to keep. 
        // If f(a) and f(midpoint) have the SAME sign, the root is NOT between a and midpoint.
        // Therefore, the root lies in the right half [midpoint, b].
        if (f(a) * yValue > 0){
            a = midpoint; // Move the left boundary up
        }
        else{
            b = midpoint; // Move the right boundary down
        }
    }
    return false; // Failed to converge within max iterations
}


// 2. Regula Falsi (False Position) Method

// Algorithm: Similar to bisection, but estimates the root by finding the x-intercept 
// of the line connecting (a, f(a)) and (b, f(b)).
bool regula_falsi(std::function<double(double)> f, double a, double b, double *root) {
    for(int i = 0; i < 1e6; i++){
        
        // Linear Interpolation Formula:
        // Finds the point where the line connecting bounds crosses the x-axis.
        double midpoint = a - (f(a)*(b - a))/(f(b) - f(a));
        double yValue = f(midpoint);

        // Convergence Check
        if (std::abs(yValue) < 1e-6){
            *root = midpoint;
            return true;
        }

        // Standard bracket narrowing logic (same as Bisection)
        // If signs are opposite, the root is between a and midpoint.
        if (f(a) * yValue < 0){
            b = midpoint;
        }
        else{
            a = midpoint;
        }
    }
    return false; 
}


// 3. Newton-Raphson Method

// Algorithm: Uses the derivative (tangent line) to project the next guess.
// Requires: Function f, Derivative g, Bounds [a,b] for safety, Initial guess c.
bool newton_raphson(std::function<double(double)> f, 
                    std::function<double(double)> g, 
                    double a, double b, double c, 
                    double *root) {

    double x_n = c; // Current guess

    for(int i = 0; i < 1e6; i++){
        double yValue = f(x_n);
        double dyValue = g(x_n); // Value of the derivative at x_n

        // Convergence Check
        if (std::abs(yValue) < 1e-6){
            *root = x_n;
            return true;
        }
        
        // Critical Safety Check:
        // If the derivative is 0, the tangent is horizontal and never intersects the x-axis.
        // Division by zero would occur in the update step.
        if(std::abs(dyValue) < 1e-6){
            return false;
        }

        // Bounds Check:
        // Newton's method can be unstable and shoot off to infinity. 
        // This ensures the guess stays within the expected interval [a, b].
        if (x_n < a || x_n > b){ 
            return false;
        }

        // Update Step: x(n+1) = x(n) - f(x) / f'(x)
        x_n = x_n - (f(x_n)/g(x_n));
    }

    return false; 
}


// 4. Secant Method

// Algorithm: An approximation of Newton-Raphson that uses a finite difference 
// between two points instead of an explicit derivative.
// Note: Uses 'a' as x_prev and 'c' as x_current. 'b' appears unused in loop logic.
bool secant(std::function<double(double)> f, double a, double b, double c, double *root) {

     x_current = c;
    double x_prev = a;
    double x_next;

    for(int i = 0; i < 1e6; i++){

        // Secant Formula:
        // Approximates derivative as: (f(x_curr) - f(x_prev)) / (x_curr - x_prev)
        // Then applies Newton's update rule.
        x_next = x_current - f(x_current)* ((x_current - x_prev)/(f(x_current)- f(x_prev)));

        // Convergence Check (Difference between steps):
        // This checks if the x-values have stopped changing significantly.
        if (std::abs(x_next - x_current) < 1e-6){
            *root = x_next;
            return true;
        }
        
        // Shift variables for next iteration
        x_prev = x_current;
        x_current = x_next;
    }
    return false;  
}