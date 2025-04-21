Program         ::= Statement Program
                  | ε        

Statement       ::= Declaration
                  | Assignment
                  | Increment
                  | Decrement
                  | Print
                  | Expr ';'


Declaration     ::= Type ID '=' Expr ';'

Assignment      ::= ID '=' Expr ';'

Increment       ::= ID '++' ';'

Decrement       ::= ID '--' ';'

Print           ::= 'print' '(' Expr ')' ';'

Expr            ::= Expr '+' Term
                  | Expr '-' Term
                  | Term

Term            ::= Term '*' Factor
                  | Term '/' Factor
                  | Factor

Factor          ::= '(' Expr ')'
                  | ID
                  | INT
                  | FLOAT

Type            ::= 'int'
                  | 'float'
