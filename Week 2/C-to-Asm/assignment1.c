//
// Assignment write the following C program in AVR Assembly
//
int a = 3;
int b = 7;
int c;

int main(void) {
    do {
        a = a-1;
        b = b-1;
        c = a;
        
        if (b == 6) {
            b = b-a;
        } else {
            if (b == 3) {
                a = a-1;
            }
        }
    } while (a > 0);
}