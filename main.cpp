
#include "std_lib_facilities.h"

const char number = '8';
const char quit = 'q';
const char print = ';';
const string declkey = "let";
const string constkey = "const";


const string prompt = "> ";
const string result = "= ";


class Token {
public:
    char kind;
    double value;

    Token(char ch)
            : kind(ch), value(0) {}

    Token(char ch, double val)
            : kind(ch), value(val) {}
};

class Token_stream {
public:
    Token_stream();

    Token get();

    void putback(Token t);

    void ignore(char c);

private:
    bool full;
    Token buffer;
};


Token_stream ts;

double expression();

double term();

double primary();

void clean_up_mess();


void calculate() {
    while (cin) {
        try {
            cout << prompt;
            Token t = ts.get();
            while (t.kind == print) t = ts.get();
            if (t.kind == quit) return;
            ts.putback(t);
            cout << result << expression() << endl;
        }
        catch (exception &e) {
            cerr << e.what() << endl;
            clean_up_mess();
        }
    }
}

int main() {
    try {
        calculate();
//    try {
//        double val = 0;
//
//        while (cin) {
//            Token t = ts.get();
//            if (t.kind == 'q')
//                break;
//            if (t.kind == ';')
//                cout << "= " << val << '\n4';
//            else
//                ts.putback(t);
//            val = expression();
//        }

//    while (cin)
//        cout << expression() << '\n';
//    keep_window_open("~0");
        return 0;
    }
    catch (exception &e) {
        cerr << e.what() << endl;
        keep_window_open("~1");
        return 1;
    }
    catch (...) {
        cerr << "exception \n";
        keep_window_open("~2");
        return 2;
    }

}

double expression() {
    double left = term();
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
            case '+':
                left += term();
                t = ts.get();
                break;
            case '-':
                left -= term();
                t = ts.get();
                break;
            default:
                ts.putback(t);
                return left;
        }
    }
}

double term() {
    double left = primary();
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
            case '*':
                left *= primary();
                t = ts.get();
                break;
            case '/': {
                double d = primary();
                if (d == 0) error("divide by zero");
                left /= d;
                t = ts.get();
                break;
            }

            case '%': {
                int i1 = narrow_cast<int>(left);
                int i2 = narrow_cast<int>(term());
                if (i2 == 0) error("%: divide by zero");
                left = i1 % i2;
                t = ts.get();
                break;
            }
            default:
                ts.putback(t);
                return left;
        }
    }
}

double primary() {
    Token t = ts.get();
    switch (t.kind) {
        case '(': {
            double d = expression();
            t = ts.get();
            if (t.kind != ')') error("')' expected");
            return d;
        }
        case '8':
            return t.value;
        case '-':
            return -primary();
        case '+':
            return primary();
        default:
            error("primary expected");
    }
}


Token_stream::Token_stream()
        : full(false), buffer(0) {
}


void Token_stream::putback(Token t) {
    if (full) error("putback() into a full buffer");
    buffer = t;
    full = true;
}


Token Token_stream::get() {
    if (full) {
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch;

    switch (ch) {
        case quit:
        case print:
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '=':
            return Token(ch);
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            cin.putback(ch);
            double val;
            cin >> val;
            return Token(number, val);
        }
        default:
            error("Bad token");
    }
}

void Token_stream::ignore(char c) {

    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;


    char ch = 0;
    while (cin >> ch)
        if (ch == c) return;
}

Token get_token() {
    char ch;
    cin >> ch;

    switch (ch) {
        case ';':
        case 'q':
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
            return Token(ch);        // let each character represent itself
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            cin.putback(ch);
            double val;
            cin >> val;
            return Token(number, val);
        }
        default:
            error("Bad token");
    }
}

void clean_up_mess() {
    ts.ignore(print);
}


