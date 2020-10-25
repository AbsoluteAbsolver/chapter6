
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
const char number = '8'; // t.kind==number means that t is a number Token
const char quit = 'x'; // t.kind==quit means that t is a quit Token
const char print = '='; // t.kind==print means that t is a print Token
const string prompt = "> ";
const string result = "= "; // used to indicate that what follows is a result
//------------------------------------------------------------------------------

class Variable {
public:
    string name;
    double value;
};

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
    Token get();      // get a Token (get() i  s defined elsewhere)
    void putback(Token t);    // put a Token back
    void ignore(char c); // discard characters up to and including a c
private:
    bool full;        // is there a Token in the buffer?
    Token buffer;     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

void Token_stream::ignore(char c)
// c represents the kind of Token
{
    // first look in buffer:
    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;
    // now search input:
    char ch = 0;
    while (cin >> ch)
        if (ch == c) return;
}

// the constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
    :full(false), buffer(0)    // no token in buffer
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
    case print:    // for "print"
    case quit:    // for "quit"
    case '(': case ')': 
    case '+': case '-': 
    case '*': case '/': 
    case '{':
    case '}': 
    case '!': 
    case '%':
        return Token(ch);        // let each character represent itself
    case '.':
    case '0': case '1': case '2': case '3': case '4':
        // ERROR.3.logic case '5': case '6': case '7': case '9':
    case '5': case '6': case '7': case'8': case '9':
    {
        cin.putback(ch);         // put digit back into the input stream
        double val;
        cin >> val;              // read a floating-point number
        return Token(number, val);   // let '8' represent "a number"
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

//calculates factorial of numbers only valid for 0 and positive integers
int factorial(double val) {
    int n = int(val);
    double intpart;
    if (modf(val, &intpart) != 0.0) error("number is not an int before factorial");
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
    case '(': case '{': {    // handle '(' expression ')'
        double d = expression();
        t = ts.get();
        if (t.kind == ')' || t.kind == '}') {
            left = d;
            break;
        }
        error("')' or '}' expected");
    }
    case number:         
        left = t.value;  // return the number's value
        break;
    case '-':            // if negetive return negative of the primary
        left = -primary();  
        break;
    case '+':            
        left = primary();  
        break;
    default:
        error("primary expected");
    

}
    while (true) {
        t = ts.get();
        if (t.kind == '!') left = factorial(left);
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
            break;
        case '/':
        {
            double d = primary();
            if (d == 0) error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        case '%':
        { 
         double d = primary();
        if (d == 0) error("divide by zero");
        left = fmod(left, d);
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

void clean_up_mess()
{
    ts.ignore(print);
}

//------------------------------------------------------------------------------

void calculate() // expression evaluation loop
{
    while (cin) {
        try {
            cout << prompt;
            Token t = ts.get();
            while (t.kind == print) t = ts.get(); // first discard all “prints”
            if (t.kind == quit) return;
            ts.putback(t);
            cout << result << expression() << '\n';
        }
        catch (exception& e) {
            cerr << e.what() << '\n'; // write error message
            clean_up_mess();
        }
    }
}

//------------------------------------------------------------------------------

int main()
try
{
    calculate();
    keep_window_open();
    return 0;
}
catch (exception& e) {
    cerr << "error: " << e.what() << '\n';
    keep_window_open("``");
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n";
    keep_window_open("``");
    return 2;
}

//------------------------------------------------------------------------------