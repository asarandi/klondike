#include "klondike.h"

int main()//(int ac, char **av)
{
    char *deck = "4283T3215393K254Q3845213T464K3Q124413482433233T172K1T2J12322113161629471514473J3J481Q4927412Q2J2K4631491";
    t_board *b = stob(deck);
    print(b);
    printf("num_moves() = %d\n", num_moves(b));

    return 0;
}
