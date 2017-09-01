param s;
set Y := 1..s;
set X := 1..s;
param board{Y, X} integer >= 0;

var knight{Y, X} binary;
var delta{Y, X};
var absdelta{Y, X} >= 0;

minimize SCORE: sum{y in Y, x in X}
    absdelta[y, x]
    ;
s.t. DELTA{y in Y, x in X}: delta[y, x] =
    board[y, x]
    - (if y-1 >= 1 and x+2 <= s then knight[y-1, x+2] else 0)
    - (if y-2 >= 1 and x+1 <= s then knight[y-2, x+1] else 0)
    - (if y-2 >= 1 and x-1 >= 1 then knight[y-2, x-1] else 0)
    - (if y-1 >= 1 and x-2 >= 1 then knight[y-1, x-2] else 0)
    - (if y+1 <= s and x-2 >= 1 then knight[y+1, x-2] else 0)
    - (if y+2 <= s and x-1 >= 1 then knight[y+2, x-1] else 0)
    - (if y+2 <= s and x+1 <= s then knight[y+2, x+1] else 0)
    - (if y+1 <= s and x+2 <= s then knight[y+1, x+2] else 0)
    ;
s.t. UPPER{y in Y, x in X}: absdelta[y, x] >= + delta[y, x];
s.t. LOWER{y in Y, x in X}: absdelta[y, x] >= - delta[y, x];

end;
