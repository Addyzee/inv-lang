Program         ::= Statement Program
                  | ε        

Statement       ::= Declaration
                  | Assignment
                  | Print
                  | Expr ';'

Declaration     ::= Type ID '=' Expr ';'

Assignment      ::= ID '=' Expr ';'

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
