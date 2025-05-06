#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <map>
using namespace std;

enum type_of_lex {
    LEX_NULL, LEX_AND, LEX_DO, LEX_ELSE, LEX_IF, LEX_INT,
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_WHILE, LEX_WRITE,
    LEX_GOTO, LEX_REAL, LEX_STRING,
    LEX_FIN,
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_EQ, LEX_LPAREN, LEX_RPAREN, LEX_LBRACE, LEX_RBRACE,
    LEX_ASSIGN, LEX_EQEQ, LEX_NEQ, LEX_LT, LEX_LE, LEX_GT, LEX_GE,
    LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_MOD,
    LEX_NUM, LEX_REALNUM, LEX_STR, LEX_ID, LEX_LABEL
};

// вспомогательная функция для красивого вывода
string type_of_lex_to_string(type_of_lex t) {
    switch (t) {
    case LEX_NULL: return "LEX_NULL";
    case LEX_AND: return "LEX_AND";
    case LEX_DO: return "LEX_DO";
    case LEX_ELSE: return "LEX_ELSE";
    case LEX_IF: return "LEX_IF";
    case LEX_INT: return "LEX_INT";
    case LEX_NOT: return "LEX_NOT";
    case LEX_OR: return "LEX_OR";
    case LEX_PROGRAM: return "LEX_PROGRAM";
    case LEX_READ: return "LEX_READ";
    case LEX_THEN: return "LEX_THEN";
    case LEX_WHILE: return "LEX_WHILE";
    case LEX_WRITE: return "LEX_WRITE";
    case LEX_GOTO: return "LEX_GOTO";
    case LEX_REAL: return "LEX_REAL";
    case LEX_STRING: return "LEX_STRING";
    case LEX_FIN: return "LEX_FIN";
    case LEX_SEMICOLON: return "LEX_SEMICOLON";
    case LEX_COMMA: return "LEX_COMMA";
    case LEX_COLON: return "LEX_COLON";
    case LEX_ASSIGN: return "LEX_ASSIGN";
    case LEX_LPAREN: return "LEX_LPAREN";
    case LEX_RPAREN: return "LEX_RPAREN";
    case LEX_LBRACE: return "LEX_LBRACE";
    case LEX_RBRACE: return "LEX_RBRACE";
    case LEX_EQ: return "LEX_EQ";
    case LEX_EQEQ: return "LEX_EQEQ";
    case LEX_NEQ: return "LEX_NEQ";
    case LEX_LT: return "LEX_LT";
    case LEX_LE: return "LEX_LE";
    case LEX_GT: return "LEX_GT";
    case LEX_GE: return "LEX_GE";
    case LEX_PLUS: return "LEX_PLUS";
    case LEX_MINUS: return "LEX_MINUS";
    case LEX_TIMES: return "LEX_TIMES";
    case LEX_SLASH: return "LEX_SLASH";
    case LEX_MOD: return "LEX_MOD";
    case LEX_NUM: return "LEX_NUM";
    case LEX_REALNUM: return "LEX_REALNUM";
    case LEX_STR: return "LEX_STR";
    case LEX_ID: return "LEX_ID";
    case LEX_LABEL: return "LEX_LABEL";
    default: return "UNKNOWN";
    }
}

// класс лексем
class Lex {
    type_of_lex t_lex;
    string value;
    int pos_lex;   // позиция в строке
    int id_val;    // номер идентификатора в TID (если это идентификатор)
    int line_lex;  // номер строки
public:
    Lex(type_of_lex t = LEX_NULL, string v = "", int line = 0, int pos = 0, int id = -1) 
        : t_lex(t), value(v), line_lex(line), pos_lex(pos), id_val(id) {}
    type_of_lex get_type() const { return t_lex; }
    string get_value() const { return value; }
    int get_line() const { return line_lex; }
    int get_pos() const { return pos_lex; }
    int get_id() const { return id_val; }
    friend ostream& operator<<(ostream& s, const Lex l) {
        s << "(" << type_of_lex_to_string(l.get_type()) << ", " << l.get_value() << ", line " << l.get_line() << ")";
        return s;
    }
};

struct Ident {
    string name;            // Имя переменной
    type_of_lex type;       // Тип (int, real, string)
    bool declared;          // true, если объявлена

    Ident(string n = "") : name(n), type(LEX_NULL), declared(false) {}
};

// таблица идентификаторов 
vector<Ident> TID;
// функция добавляет идентификатор, если его там ещё нет
int put(const string& buf) {
    for (size_t i = 0; i < TID.size(); ++i)
        if (TID[i].name == buf)
            return i;
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}



// чтение символов из файла, распознавание лексем
class Scanner {
    FILE* fp;
    char c;
    int pos_counter = 0;
    int start_pos = 0;
    void gc() {
        c = fgetc(fp); 
        if (c != EOF) {
            pos_counter++;
            if (c == '\n') {
                line_counter++;
                pos_counter = 0;
            }
        }
    }

    int look(const string& buf, const char* list[]) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i]) return i;
            ++i;
        }
        return -1;
    }
public:
    static int line_counter;
    static const char* TW[], * TD[];
    Scanner(const char* program) {
        if (!(fp = fopen(program, "r")))
            throw "Ошибка открытия файла!";
    }
    Lex get_lex();
};

int Scanner::line_counter = 1;

// служебные слова
const char* Scanner::TW[] = {
    "", "and", "do", "else", "if", "int",
    "not", "or", "program", "read", "then", "while", "write",
    "goto", "real", "string",  NULL
};

// операторы
const char* Scanner::TD[] = {
    "",";", ",", ":", ":=", "(", ")", "{", "}", "=", "==", "!=", "<", "<=", ">", ">=",
    "+", "-", "*", "/", "%", NULL
};

struct ParseError {
    string message;
    int line;
    int pos;

    ParseError(string msg, int l, int p) : message(msg), line(l), pos(p) {}
};

// автомат, строящий лексемы
Lex Scanner::get_lex() {
    enum state { S, IDEN, NUMB, STR, ALE, NEQ, COMM }; // состояния автомата
    string buf;
    int j;
    state CS = S;
    while (true) {
        gc();
        switch (CS) {
        case S:
            if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                //if (c == '\n') line_counter++;
                // cout << line_counter << "\n" << endl;
                continue;
            }
            start_pos = pos_counter;
            if (isdigit(c)) { buf += c; CS = NUMB; }
            else if (c == '"') { buf.clear(); CS = STR; }
            else if (isalpha(c)) { buf += c; CS = IDEN; }
            else if (c == ':' || c == '<' || c == '>' || c == '=') { buf += c; CS = ALE; }
            else if (c == EOF) return Lex(LEX_FIN, "EOF", line_counter);
            else if (c == '!') { buf += c; CS = NEQ; }
            else if (c == '/') {
                gc();
                if (c == '*') {
                    CS = COMM;
                }
                else{
                    ungetc(c, fp);
                    return Lex(LEX_SLASH, "/");
                }
            }
            else {
                buf += c;
                if ((j = look(buf, TD)) != -1)
                    return Lex((type_of_lex)(j + LEX_FIN), buf, line_counter);
                else
                    throw c;
            }
            break;
        
        case NUMB:
            while (isdigit(c)) { buf += c; gc(); }
            if (c == '.') {
                buf += c;
                gc();
                if (isdigit(c)) {
                while (isdigit(c)) { buf += c; gc(); }
                ungetc(c, fp);
                return Lex(LEX_REALNUM, buf, line_counter);
                } 
                else {
                    throw ParseError("После точки нет цифр", line_counter, start_pos);
                }
            } else 
            {
                ungetc(c, fp);
                return Lex(LEX_NUM, buf, line_counter);
            }

        case STR:
            while (c != '"') { buf += c; gc(); }
            return Lex(LEX_STR, buf, line_counter);
        
        case IDEN:
            while (isalnum(c)) { buf += c; gc(); }
            if (c == ':'){
                buf+=c;
                return Lex(LEX_LABEL, buf, line_counter);
            }
            ungetc(c, fp);
            if ((j = look(buf, TW)) != -1) {   // служ. слово
                return Lex((type_of_lex)j, buf, line_counter);
            } 
            else {
                int id = put(buf);     
                return Lex(LEX_ID, buf, line_counter, start_pos, id);
            }
        
        case ALE:
            if (c == '=') {
                buf += c; 
                gc();
            }
            j = look(buf, TD);
            if (j == -1)
                throw buf[0];
            return Lex((type_of_lex)(j + LEX_FIN), buf, line_counter);
        
        case NEQ:
            if (c == '=') { buf += c; gc(); j = look(buf, TD); return Lex(LEX_NEQ, buf, line_counter); }
            else throw '!';
        
        case COMM:
            while (true) {
                gc();
                if (c == '\n'){
                    line_counter++;
                }
                else if (c == '*') {
                    gc();
                    if (c == '\n'){
                        line_counter++;
                    }
                    else if (c == '/') {
                        // cout << "m here " << line_counter << "\n" << endl;
                        CS = S;
                        break;
                    }
                    else{
                        throw c;
                    }
                }
            if (c == EOF) throw ParseError("Незакрытый комментарий", line_counter, start_pos);
            }
            break;
        }
    }
}

//синтаксический анализатор
class Parser {
    Lex curr_lex;
    type_of_lex c_type;
    Scanner scan;
    stack<Lex> saved_lex_stack;
    bool flag_prev = false;
    bool flag_next = false;
    Lex prev_lex, next_lex;

    stack<int> st_int; // индексы идентификаторов
    stack<type_of_lex> st_lex; // типы
    // Таблица меток: имя метки -> была ли определена
    map<string, bool> label_table;

public:
    Parser(const char* program) : scan(program) {}
    void analyze();

private:
     void clean_stack() {
     while (!st_lex.empty())
         st_lex.pop();
     }
    void next() {
        // if (flag_prev) {
        //     next_lex = curr_lex;
        //     curr_lex = prev_lex;
        //     flag_prev = false;
        //     flag_next = true;
        // }
        // else if (flag_next) {
        //     prev_lex = curr_lex;
        //     curr_lex = next_lex;
        //     flag_next = false;
        // }
        // else {
            // prev_lex = curr_lex;
            curr_lex = scan.get_lex();
        // }
        c_type = curr_lex.get_type();
        cout << "Next token: " << type_of_lex_to_string(c_type) << " (" << curr_lex.get_value() << ")" << endl;
    }
    
    
    void save_lex() {
        saved_lex_stack.push(curr_lex);
    }
    
    void restore_lex() {
        if (!saved_lex_stack.empty()) {
            curr_lex = saved_lex_stack.top();
            c_type = curr_lex.get_type();
            saved_lex_stack.pop();
        } else {
            throw ParseError("Нет сохранённой лексемы для восстановления", curr_lex.get_line(), curr_lex.get_pos());
        }
    }
    
    

    // Нетерминалы

    // P → program B
    void P();    // Программа

    // B → { { [ int | real | string ] D } S }
    void B();    // Блок

    // D → I { , I } ;
    void D();    // Описание переменных

    // I → id [ = CONST ]
    void I (type_of_lex var_type);

    // S → 
    //      if ( E ) S [ else S ]
    //    | while ( E ) S
    //    | do S while ( E ) ;
    //    | goto id ;
    //    | read ( id ) ;
    //    | write ( E { , E } ) ;
    //    | { B }
    //    | id : S
    //    | A ;
    //    | ;
    void S();    // Оператор

    // A → id = A | E
    //    (правоассоциативное присваивание)
    void A();    // Присваивание

    // E → E1 { (== | != | < | <= | > | >=) E1 }
    void E();    // Логическое выражение

    // E1 → T { ( + | - | or ) T }
    void E1();   // Арифметическое выражение

    // T → F { ( * | / | mod | and ) F }
    void T();    

    // F → 
    //      id
    //    | num
    //    | realnum
    //    | str
    //    | not F
    //    | ( A )
    //    | + F
    //    | - F
    void F();    


    // Проверки
    void declare(type_of_lex type);
    void check_id();
    void check_id_in_read();
    void check_op();
    void check_not();
    void eq_type(bool keep_result = true);
    void eq_bool();
    void check_umup();
};


void Parser::analyze() {
    next();
    P();
    if (c_type != LEX_FIN)
        throw curr_lex;
    cout << "УРААА!!" << endl;
}

void Parser::P() {
    if (c_type != LEX_PROGRAM)
        throw ParseError("Ожидалась 'program'", curr_lex.get_line(), curr_lex.get_pos());
    next();
    B();
}

void Parser::B() {
    if (c_type != LEX_LBRACE)
        throw ParseError("Ожидалась открывающая скобка '('", curr_lex.get_line(), curr_lex.get_pos());;
    next();
    while (c_type == LEX_INT || c_type == LEX_REAL || c_type == LEX_STRING)
        D();
    clean_stack();
    while (c_type != LEX_RBRACE && c_type != LEX_FIN)
        S();
    if (c_type != LEX_RBRACE)
        throw ParseError("Ожидалась закрывающая скобка ')'", curr_lex.get_line(), curr_lex.get_pos());;
    next();
}

void Parser::D() {
    while (c_type == LEX_INT || c_type == LEX_REAL || c_type == LEX_STRING){
        type_of_lex var_type = c_type;
        next();
        I(var_type);
    //  st_int.push(put(curr_lex.get_value()));
    //  st_lex.push(var_type);
    //  next();

    // if (c_type == LEX_ASSIGN) {
    //     next();
    //     E();
    //     eq_type(false);
    // }
        while (c_type == LEX_COMMA) {
            next();
            I(var_type);
         }
        if (c_type != LEX_SEMICOLON)
            throw ParseError("Ожидалась ';'1", curr_lex.get_line(), curr_lex.get_pos());
        next();
    }
}


void Parser::I(type_of_lex var_type){
    if (c_type != LEX_ID) throw ParseError("Ожидался идентификатор", curr_lex.get_line(), curr_lex.get_pos());
    int id = curr_lex.get_id();
    if (TID[id].declared)
            throw ParseError("Повторное объявление переменной", curr_lex.get_line(), curr_lex.get_pos());
    TID[id].declared = true;
    TID[id].type = var_type;
    next();
    if (c_type == LEX_ASSIGN){
        next();
        if (var_type == LEX_INT){
            int sign = 1;
            if (c_type == LEX_PLUS){
                next();
            }
            else if (c_type == LEX_MINUS){
                sign = -1;
                next();
            }
            if (c_type!= LEX_NUM)  throw ParseError("Ожидалось целое число", curr_lex.get_line(), curr_lex.get_pos());
        
        st_int.push(id);
        st_lex.push(var_type);
        next();
    }
    else if (var_type == LEX_REAL) {
        int sign = 1;
            if (c_type == LEX_PLUS) {
                next();
            } else if (c_type == LEX_MINUS) {
                sign = -1;
                next();
            }
        if (c_type != LEX_REALNUM)
            throw ParseError("Ожидалось вещественное число", curr_lex.get_line(), curr_lex.get_pos());
        st_int.push(id);
        st_lex.push(LEX_REAL);
        next();
    }
    else if (var_type == LEX_STRING) {
        if (c_type != LEX_STR)
            throw ParseError("Ожидалась строка", curr_lex.get_line(), curr_lex.get_pos());

         st_int.push(id);
         st_lex.push(LEX_STRING);

        next();
    } else 
        throw ParseError("Несовместимый тип для присваивания", curr_lex.get_line(), curr_lex.get_pos());
    } 
    else {
        // Если просто объявили переменную без присваивания
         st_int.push(id);
        st_lex.push(var_type);
    }
}

void Parser::S() {
    // cout << "Сейчас анализируем оператор: " << type_of_lex_to_string(c_type) 
    // << " " << curr_lex.get_value() 
    // << " (строка " << curr_lex.get_line() << ")" << endl;
     if (c_type == LEX_LABEL) {
         string label_name = curr_lex.get_value();
         if (label_table.count(label_name)) {
             throw ParseError("Повторное определение метки '" + label_name + "'", 
                            curr_lex.get_line(), curr_lex.get_pos());
         }
         label_table[label_name] = true;
         next();
        //  if (c_type != LEX_COLON){
        //     throw curr_lex;
        //  }
         return S();
     }
    if (c_type == LEX_IF) {
        next();
        if (c_type != LEX_LPAREN)
            throw ParseError("Ожидалась открывающая скобка '(' после if", curr_lex.get_line(), curr_lex.get_pos());
        next();
        E();
        if (c_type != LEX_RPAREN)
            throw ParseError("Ожидалась закрывающая скобка ')' после условия", curr_lex.get_line(), curr_lex.get_pos());
        next();
        S();
        if (c_type == LEX_ELSE) {
            next();
            S();
        }
        else throw ParseError("Неверное описание if else", curr_lex.get_line(), curr_lex.get_pos());
    } 
    else if (c_type == LEX_WHILE) {
        next();
        if (c_type != LEX_LPAREN) throw ParseError("Ожидалась открывающая скобка '('", curr_lex.get_line(), curr_lex.get_pos());
        next();
        E();
        if (c_type != LEX_RPAREN) throw ParseError("Ожидалась закрывающая скобка ')'", curr_lex.get_line(), curr_lex.get_pos());
        next();
        S();  // Тело цикла
    }
    else if (c_type == LEX_DO) {
        next();
        S();  // Тело цикла
        if (c_type != LEX_WHILE)
            throw ParseError("Неверное описание do while", curr_lex.get_line(), curr_lex.get_pos());
        next();
        if (c_type != LEX_LPAREN)
            throw ParseError("Ожидалась открывающая скобка '('", curr_lex.get_line(), curr_lex.get_pos());
        next();
        E();
        if (c_type != LEX_RPAREN)
            throw ParseError("Ожидалась закрывающая скобка ')'", curr_lex.get_line(), curr_lex.get_pos());
        next();
         if (c_type != LEX_SEMICOLON)
            throw ParseError("Ожидалась ';'2", curr_lex.get_line(), curr_lex.get_pos());
         next();
    } else if (c_type == LEX_GOTO) {
        next();
        if (c_type != LEX_ID)
            throw ParseError("Ожидался идентификатор", curr_lex.get_line(), curr_lex.get_pos());   
        string label_name =curr_lex.get_value()+":";
        // Проверка существования метки
        if (!label_table.count(label_name)) {
            throw ParseError("Использование неопределённой метки '" + label_name + "'", 
                           curr_lex.get_line(), curr_lex.get_pos());
        }    
        next();
        if (c_type != LEX_SEMICOLON)
            throw ParseError("Ожидалась ';'3", curr_lex.get_line(), curr_lex.get_pos());
        next();
    } else if (c_type == LEX_READ) {
        next();
        if (c_type != LEX_LPAREN)
        throw ParseError("Ожидалась открывающая скобка '('", curr_lex.get_line(), curr_lex.get_pos());
        next();
        if (c_type != LEX_ID)
            throw ParseError("Ожидался идентификатор", curr_lex.get_line(), curr_lex.get_pos());
        check_id_in_read();
        next();
        if (c_type != LEX_RPAREN)
        throw ParseError("Ожидалась закрывающая скобка ')'", curr_lex.get_line(), curr_lex.get_pos());
        next();
        if (c_type != LEX_SEMICOLON)
            throw ParseError("Ожидалась ';'4", curr_lex.get_line(), curr_lex.get_pos());
        next();
    } else if (c_type == LEX_WRITE) {
        next();
        if (c_type != LEX_LPAREN)
        throw ParseError("Ожидалась открывающая скобка '('", curr_lex.get_line(), curr_lex.get_pos());
        next();
        E();
        while (c_type == LEX_COMMA) {
            next();
            E();
        }
        if (c_type != LEX_RPAREN)
        throw ParseError("Ожидалась закрывающая скобка ')'", curr_lex.get_line(), curr_lex.get_pos());
        next();
        if (c_type != LEX_SEMICOLON)
            throw ParseError("Ожидалась ';'5", curr_lex.get_line(), curr_lex.get_pos());
        next();
    } else if (c_type == LEX_ID) {
        A();
        if (c_type != LEX_SEMICOLON)
            throw ParseError("Ожидалась ';'6", curr_lex.get_line(), curr_lex.get_pos());
        next();
    }
     else if (c_type == LEX_LBRACE) {
        B();
    } 
    else if (c_type == LEX_SEMICOLON) {
        if (prev_lex.get_type() == LEX_RPAREN) {
            throw ParseError("После условия в while, if или do нельзя использовать пустой оператор ';'", curr_lex.get_line(), curr_lex.get_pos());
        }
        next();
    } 
    else {
        cout << "IN S\n" << endl;
        throw curr_lex;
    }  
}

void Parser::A() {
    cout << "WE HERER " << curr_lex.get_value() << endl;
    int left_id = curr_lex.get_id();
    type_of_lex left_type = TID[left_id].type;
    save_lex();
    next();
    if (c_type == LEX_ASSIGN) {
        next();
            cout << "ltype  " << left_type << "\n" << endl;
            st_lex.push(left_type);
            if (c_type == LEX_ID) {
                A(); // разбор правой части (возможно, ещё одно присваивание)
            } else {
                E();
            }
        }
    else {
        if (saved_lex_stack.empty()){
            throw ParseError("Ожидалось '='", curr_lex.get_line(), curr_lex.get_pos());
        }
        cout << "aaa restoring " << curr_lex.get_value() << endl;
        cout << "WE HERER after assign " << curr_lex.get_value() << endl;
        E();
    }
}

void Parser::E() {  //логические выражения
    // cout << "IN E\n" << endl;
    E1();
    while (c_type == LEX_EQEQ || c_type == LEX_NEQ || c_type == LEX_LT ||
           c_type == LEX_LE || c_type == LEX_GT || c_type == LEX_GE) {
        type_of_lex op = c_type;
        next();
        E1();
        st_lex.push(op);
        check_op();
    }
    while (c_type == LEX_OR) {
        type_of_lex op = c_type;
        next();
        E();
        st_lex.push(op);
        check_op();
    }
}


void Parser::E1() { //для +-
    // cout << "IN E1\n" << endl;
    T();
    cout << "mkay  c type and c val \n" << type_of_lex_to_string(curr_lex.get_type()) << " " << curr_lex.get_value() << " " << endl;
    while (c_type == LEX_PLUS || c_type == LEX_MINUS) {
        cout << "mkay1\n" << endl;
        type_of_lex op = c_type;
        next();
        T();
        st_lex.push(op);
        check_op();
    }
}

void Parser::T() {
    F();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_MOD || c_type == LEX_AND) {
        type_of_lex op = c_type;
        next();
        F();
        st_lex.push(op);      
        check_op();
    }
}


void Parser::F() {
    cout << "IN F  now lex " << curr_lex.get_value() << "  c type" << c_type << endl;
    if (c_type == LEX_ID){
        cout << "I identified lex_id\n " << endl;
        int id_num = put(curr_lex.get_value());
        cout << TID[id_num].type << "\n" << endl;
        st_lex.push(TID[id_num].type);
        stack<type_of_lex> temp = st_lex;
    while (!temp.empty()) {
        cout << type_of_lex_to_string(temp.top()) << " ";
        temp.pop();
    }
    }
    else if (c_type == LEX_NUM || c_type == LEX_REALNUM || c_type == LEX_STR) {
        cout << "Yay numba \n" << endl;
        st_lex.push(
            c_type == LEX_NUM ? LEX_INT :
            c_type == LEX_REALNUM ? LEX_REAL :
            LEX_STRING
        );
        next();
    } else if (c_type == LEX_NOT) {
        next();
        F();
        check_not();
    } else if (c_type == LEX_MINUS || c_type == LEX_PLUS) {
        cout << "WELL  HAHHA\n" << endl;
        next();
        
        F();
        check_op();
        // check_umup();
    } else if (c_type == LEX_LPAREN) {
        next();
        E();
        if (c_type != LEX_RPAREN)
            throw ParseError("Ожидалась закрывающая скобка ')'", curr_lex.get_line(), curr_lex.get_pos());
        next();
    } else {
        cout << "IN F\n" << endl;
        throw curr_lex;
    }
}


void Parser::declare(type_of_lex type) {
    while (!st_int.empty()) {
        int i = st_int.top(); st_int.pop();
        if (TID[i].declared)
            throw ParseError("Повторное объявление переменной", curr_lex.get_line(), curr_lex.get_pos());
        TID[i].declared = true;
        TID[i].type = type;
    }
}


void Parser::check_id() {
    int id_num = put(curr_lex.get_value());  // Узнаем номер переменной
    if (!TID[id_num].declared)
        throw ParseError("Идентификатор не объявлен", curr_lex.get_line(), curr_lex.get_pos());
    st_lex.push(TID[id_num].type);
}


void Parser::check_id_in_read() {
    int id_num = put(curr_lex.get_value());
    if (!TID[id_num].declared)
        throw ParseError("Идентификатор не объявлен для чтения", curr_lex.get_line(), curr_lex.get_pos());
}

void Parser::check_op() {
    stack<type_of_lex> temp = st_lex;
    while (!temp.empty()) {
        cout << type_of_lex_to_string(temp.top()) << " ";
        temp.pop();
    }
    cout << endl;
    type_of_lex op = st_lex.top(); st_lex.pop();
    type_of_lex t2 = st_lex.top(); st_lex.pop();
    type_of_lex t1 = st_lex.top(); st_lex.pop();

    cout << "check_op вызван для оператора: " << type_of_lex_to_string(op) << endl;
    if (t1 != t2)
        throw ParseError("Несовпадение типов в операции " + type_of_lex_to_string(op), curr_lex.get_line(), curr_lex.get_pos());
    if (op == LEX_EQEQ || op == LEX_NEQ || op == LEX_LT ||
        op == LEX_LE || op == LEX_GT || op == LEX_GE) {
        st_lex.push(LEX_INT);
        }
    else if (op == LEX_PLUS) {
        if (t1 == LEX_STRING && t2 == LEX_STRING) {
            st_lex.push(LEX_STRING); // строковая конкатенация
            } else if (t1 == t2 && (t1 == LEX_INT || t1 == LEX_REAL)) {
                st_lex.push(t1); // сложение чисел
        }
    }
    else if (op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH) {
        if (t1 != LEX_INT && t1 != LEX_REAL)
            throw ParseError("Операция применима только к типу int/real", curr_lex.get_line(), curr_lex.get_pos());
        st_lex.push(t1);
    }
    else if (op == LEX_OR || op == LEX_AND) {
        if (t1 != LEX_INT)
            throw ParseError("Операция применима только к int", curr_lex.get_line(), curr_lex.get_pos());
        st_lex.push(LEX_INT);
    } else {
        throw ParseError("Неизвестный оператор", curr_lex.get_line(), curr_lex.get_pos());
    }
}


void Parser::check_not() {
    type_of_lex t = st_lex.top(); st_lex.pop();
    if (t != LEX_INT)
        throw ParseError("Тип в not — не логический", curr_lex.get_line(), curr_lex.get_pos());
    st_lex.push(LEX_INT);
}

void Parser::eq_type(bool keep_result) 
{
    // cout << "eq_type(): стек перед сравнением типов: ";
    stack<type_of_lex> temp = st_lex;
    while (!temp.empty()) {
        cout << type_of_lex_to_string(temp.top()) << " ";
        temp.pop();
    }
    cout << endl;

    if (st_lex.size() < 2)
        throw ParseError("Ошибка: стек типов содержит меньше 2 элементов перед eq_type()", curr_lex.get_line(), curr_lex.get_pos());
    type_of_lex t1 = st_lex.top(); st_lex.pop();
    type_of_lex t2 = st_lex.top(); st_lex.pop();
    if (t1 != t2)
        throw ParseError("Несовпадение типов в присваивании", curr_lex.get_line(), curr_lex.get_pos());
    if (keep_result)
        st_lex.push(t1);
}

void Parser::eq_bool() {
    if (st_lex.top() != LEX_INT)
        throw ParseError("Выражение не логическое", curr_lex.get_line(), curr_lex.get_pos());
    st_lex.pop();
}

void Parser::check_umup(){
    if (st_lex.top() != LEX_INT)
        throw ParseError("Нельзя применить унарный + или - к string", curr_lex.get_line(), curr_lex.get_pos());
    st_lex.pop();
}


int main() {
    try {
        Parser parser("prog1.txt"); // имя файла с программой
        parser.analyze();
    } catch (const ParseError& err) {
        cout << "Ошибка: " << err.message 
             << " (строка " << err.line << ", позиция " << err.pos << ")" << endl;
    } catch (const char* err) {
        cout << "Error: " << err << endl;
    } catch (Lex l) {
        cout << "Ошибка на строке " << l.get_line() << ", позиция " << l.get_pos()
             << ": " << type_of_lex_to_string(l.get_type()) << " " << l.get_value() << endl;
    } catch (char c) {
        cout << "Непредвиденный символ: '" << c << "' (код: " << int(c) << ")" << endl;
    }
    return 0;
}
