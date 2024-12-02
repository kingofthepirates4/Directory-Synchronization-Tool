#include "dirsync.h"
//  CONVERT A FILENAME PATTERN (a glob) TO A REGULAR EXPRESSION.
//  FILENAME PATTERNS MAY NOT INCLUDE DIRECTORY SEPARATORS.
//	ON SUCCESS - A REGULAR EXPRESSION WILL BE RETURNED IN DYNAMICALLY
//		     ALLOCATED MEMORY.
//	ON FAILURE - A NULL POINTER WILL BE RETURNED.

char *glob2regex(char *glob)
{
    char *re = NULL;

    if(glob != NULL) {
	re	= calloc(strlen(glob)*2 + 4, sizeof(char));
        if(re == NULL) {
	    return NULL;
	}

	char *r	= re;

	*r++	= '^';
	while(*glob != '\0')
	    switch (*glob) {
		case '.' :
		case '\\':
		case '$' : *r++ = '\\'; *r++ = *glob++;	break;
		case '*' : *r++ = '.';  *r++ = *glob++;	break;
		case '?' : *r++ = '.'; glob++;		break;
		case '/' : free(re);
			   re	= NULL;
			   break;
		default  : *r++ = *glob++;
			   break;
	    }
	if(re) {
	    *r++	= '$';
	    *r		= '\0';
	}
    }
    return re;
}

int matchesPatterns(const char* string, const RegexPatterns* regexPatterns) {
    for (size_t i = 0; i < regexPatterns->count; i++) {
        regex_t regex;
		printf("string: %s\n",string);
		printf("pattern: %s\n",regexPatterns->patterns[i]);

        if (regcomp(&regex, regexPatterns->patterns[i], REG_EXTENDED | REG_NOSUB) != 0) {
            perror("regcomp");
            return 0;  // Failed to compile regex
        }
		printf("regex: %s\n",regex);
        int result = regexec(&regex, string, 0, NULL, 0);
		printf("result: %d\n",result);
        regfree(&regex);

        if (result == 0) {
            return 1;  // Match found
        } else if (result != REG_NOMATCH) {
            perror("regexec");
        }
    }

    return 0;  // No match found
}