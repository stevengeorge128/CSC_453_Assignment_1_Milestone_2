/* Test: keywords, identifiers, whitespace, comments, operators */

int main() {
    int x = 10;
    int y2 = x + 5;

    /* comment in the middle */
    if (x >= y2) {
        return 1;
    } else {
        while (x != 0 && y2 <= 20 || x == 5) {
            x = x - 1;
            y2 = y2 + 2;
        }
    }

    // tricky identifiers
    foo bar123 baz_99

    // single-character tokens
    ( ) { } , ; + - * /

    // two-character tokens
    == != >= <= && ||

    // division vs comment
    z = x / y2;    /* should parse '/' as opDIV above */

    // unterminated comment test (should trigger EOF error)
    /* this comment never ends...
}
