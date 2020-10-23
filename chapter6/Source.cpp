
//
// This is example code from Chapter 6.7 "Trying the second version" of
// "Software - Principles and Practice using C++" by Bjarne Stroustrup
// hello adjusting from laptop

/*
    This file is known as calculator02buggy.cpp

    I have inserted 5 errors that should cause this not to compile
    I have inserted 3 logic errors that should cause the program to give wrong results

    First try to find an remove the bugs without looking in the book.
    If that gets tedious, compare the code to that in the book (or posted source code)

    Happy hunting! xc

*/

#include "../../std_lib_facilities.h"

//------------------------------------------------------------------------------

// ERROR.1.syntax lass Token {
class Token {
public:
    char kind;        // what kind of token
    double value;     // for numbers: a value 
    Token(char ch)    // make a Token from a char
        :kind(ch), value(0) { }
    Token(char ch, double val)     // make a Token from a char and a double
        :kind(ch), value(val) { }
};

//------------------------------------------------------------------------------

class Token_stream {
public:
    Token_stream();   // make a Token_stream that reads from cin
    Token get();      // get a Token (get() is defined elsewhere)
    void putback(Token t);    // put a Token back
private:
    bool full;        // is there a Token in the buffer?
    Token buffer;     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
    :full(false), buffer(0)    // no Token in buffer
{
}

//------------------------------------------------------------------------------

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;       // copy t to buffer
    full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

// ERROR.2.syntax Token get()
Token Token_stream::get()
{
    if (full) {       // do we already have a Token ready?
        // remove token from buffer
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)

    switch (ch) {
    case '=':    // for "print"
    case 'x':    // for "quit"
    case '(': case ')': case '+': case '-': case '*': case '/': case '{':
    case '}': case '!':
        return Token(ch);        // let each character represent itself
    case '.':
    case '0': case '1': case '2': case '3': case '4':
        // ERROR.3.logic case '5': case '6': case '7': case '9':
    case '5': case '6': case '7': case'8': case '9':
    {
        cin.putback(ch);         // put digit back into the input stream
        double val;
        cin >> val;              // read a floating-point number
        return Token('8', val);   // let '8' represent "a number"
    }
    default:
        error("Bad token");
    }
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback() 

//------------------------------------------------------------------------------

double expression();    // declaration so that primary() can call expression()

//------------------------------------------------------------------------------

//calculates factorial of numbers only valid for 0 and positive numbers
int factorial(int n) {
    if (n < 0) error("negative number for factorial");
    if (n == 0) {
        return 1;
    }
    else return n * factorial(n - 1);
}

//------------------------------------------------------------------------------

// deal with numbers and parentheses and factorials
double primary()
{
    Token t = ts.get();
    double left;

    switch (t.kind) {
    case '(': case '{':    // handle '(' expression ')'
    {
        double d = expression();
        t = ts.get();
        // ERROR.4.syntax if (t.kind != ')') error("')' expected);
        if (t.kind == ')' || t.kind == '}') {
            left = d; 
            break;
        }
        error("')' or '}' expected");

    }
    case '8':            // we use '8' to represent a number
        //double val = t.value;
        //t = ts.get();
        //if (t.kind == '!')
        left = t.value;  // return the number's value
        break;
    default:
        error("primary expected");
    }

    while (true) {
        t = ts.get();
        if (t.kind == '!') left = factorial(int(left));
        else {
            ts.putback(t);
            return left;
        }
    }
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term()
{
    double left = primary();
    Token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
        case '*':
            left *= primary();
            t = ts.get();
            // ERROR.5.logic no break;
            break;
        case '/':
        {
            double d = primary();
            if (d == 0) error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        default:
            ts.putback(t);     // put t back into the token stream
            return left;
        }
    }
}

//------------------------------------------------------------------------------

// deal with + and -
double expression()
{
    // ERROR.6.syntax double left = term(;      // read and evaluate a Term
    double left = term();      // read and evaluate a Term
    Token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            // ERROR.7.logic left += term();    // evaluate Term and subtract
            left -= term();    // evaluate Term and subtract
            t = ts.get();
            break;
        default:
            ts.putback(t);     // put t back into the token stream
            return left;       // finally: no more + or -: return the answer
        }
    }
}

//------------------------------------------------------------------------------

int main()
try
{
    cout << "Welcome to our simple calculator.\n"
        << "Please enter expressions using floating-point numbers.\n"
        << "(Currently +, -, *, /, and () are supported.)\n"
        << "Evaluate the expression with = at the end. Enter x to quit.\n";

    // ERROR.8.syntax No val variable declared
    double val = 0;
    while (cin) {
        Token t = ts.get();

        if (t.kind == 'x') break; // 'q' for quit
        if (t.kind == '=') {      // ';' for "print now"
            cout << "=" << val << '\n';
        }
        else {
            ts.putback(t);
        val = expression();
        }
    }
    keep_window_open();
}
catch (exception& e) {
    cerr << "error: " << e.what() << '\n';
    keep_window_open();
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n";
    keep_window_open();
    return 2;
}

//------------------------------------------------------------------------------