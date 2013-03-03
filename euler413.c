
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define NUMBER unsigned long int
#define SYMBOL unsigned int

NUMBER k_bin[25];

struct substring
{
    unsigned int i;
    unsigned int length;
};


// return number of substrings created
int make_substrings(int n, struct substring list[])
{
    int s = 0;

    unsigned int i;
    for (i=0; i<n; i++)
    {
        unsigned int len;
        for (len=1; len<=(i+1); len++)
        {
            list[s].i = i - len + 1;
            list[s].length = len;
            s += 1;
        }
    }
    
    return s;
}

NUMBER construct(struct substring *substr, SYMBOL symbols[])
{
    NUMBER value = 0;

    int i;
    for (i=substr->i; i<substr->i+substr->length; i++)
    {
        // printf("%d <%lu, %u>\n", i, value, symbols[i]);
        value = (value * (NUMBER)10) + symbols[i];
    }

    // printf("construct: [%d, %d] -> %lu\n", substr->i, substr->length, value);
    return value;
}

void print_substr(struct substring *s)
{
    char symbols[] = "abcdefghijklmnopqrstuvwxyz";
    printf("  [%d, %d] ", s->i, s->length);
    int j;
    for (j=s->i; j<(s->i+s->length); j++)
    {
        printf("%c", symbols[j]);
    }
    printf("\n");
}

void print_substrings(struct substring list[], int count)
{
    int i;
    for (i=0; i<count; i++)
    {
        print_substr(&list[i]);
    }
}


NUMBER substring_usage_count = 0;

void filter(struct substring list[], int ssi, int list_length, unsigned int d, unsigned int div_count, SYMBOL symbols[])
{
    substring_usage_count += 1;
    // printf("Filter(%d:%d) %u %u : ", ssi, list_length, d, div_count);
    // print_substr(&list[ssi]);

    int substr_i = list[ssi].i;
    int substr_len = list[ssi].length;

    #ifdef PROGRESS
    if ((substr_len == 1) && (substr_i < 6))
    {
        int m = 0;
        fprintf(stdout, "> ");
        while (m <= substr_i)
        {
            fprintf(stdout, "%u", symbols[m]);
            m++;
        }
        fprintf(stdout, "\n");
    }
    #endif

    ssi += 1;


    if (substr_len == 1)
    {
        unsigned int r0 = (substr_i == 0) ? 1 : 0;
        unsigned int r1 = 9;

        unsigned int i;
        for (i=r0; i<=r1; i++)
        {
            symbols[substr_i] = i;

            int divisible = ((i % d) == 0) ? 1: 0;
            int current_div_count = div_count + divisible;

            if (current_div_count < 2)
            {
                if (ssi < list_length)
                {
                    filter(list, ssi, list_length, d, current_div_count, symbols);
                }
                else
                {
                    // tally
                    // printf("%u *\n", i);
                    k_bin[d] += 1;
                }
            }

        }
    }

    else
    {
        NUMBER i = construct(&list[ssi-1], symbols);

            unsigned int divisible = ((i % d) == 0) ? 1 : 0;
            unsigned int current_div_count = div_count + divisible;

            if (current_div_count < 2)
            {
                if (ssi < list_length)
                {
                    filter(list, ssi, list_length, d, current_div_count, symbols);
                }
                else if (current_div_count == 1)
                {
                    // tally
                    // printf("%lu\n", i);
                    k_bin[d] += 1;
                }
            }
    }

}


int main(int argc, char *argv[])
{
    struct substring substrings[200];

    SYMBOL symbols[25];
    int i;
    for (i=0; i<25; i++)
    {
        symbols[i] = 0;
    }


    unsigned int k0 = 1;
    unsigned int k1 = 1;

    if (argc > 1)
    {
        k0 = atoi(argv[1]);
    }
    if (argc > 2)
    {
        k1 = atoi(argv[2]);
    }

    NUMBER total = 0;

    unsigned int k;
    for (k=k0; k<=k1; k++)
    {
        substring_usage_count = 0;
        k_bin[k] = 0;
        unsigned int substring_count = make_substrings(k, substrings);
        // print_substrings(substrings, substring_count);

        filter(substrings, 0, substring_count, k, 0, symbols);

        total += k_bin[k];
        // printf("[%u, %lu, %lu] %lu\n", k, k_bin[k], total, substring_usage_count);
        printf("%u %lu %lu %lu\n", k, k_bin[k], total, substring_usage_count);
    }

    return 0;
}

