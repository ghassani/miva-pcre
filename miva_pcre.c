#include <stdio.h>
#include <string.h>
#include "miva_pcre.h"
#include "pcre/pcre.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OVECCOUNT 30
/**
* Function Parameter Data Structures
*/
MV_EL_FunctionParameter miva_regexp_parameters[] = {
	{ "expr", 4, EPF_NORMAL },
	{ "str",  3, EPF_NORMAL },
	{ "opts", 4, EPF_NORMAL },
};

/**
* miva_regexp
*
* Perform a regular expression match. Returns 0 on no match and 1 indexed offset of first match
*
* Example:
*
* <MvASSIGN NAME="g.match" VALUE="{ regexp('^Miva', 'Miva Merchant', '') }" />  - Returns 1
* <MvASSIGN NAME="g.match" VALUE="{ regexp('^miva', 'Miva Merchant', 'i') }" /> - Returns 1
* <MvASSIGN NAME="g.match" VALUE="{ regexp('^miva', 'Miva Merchant', '') }" />  - Returns 0
* <MvASSIGN NAME="g.match" VALUE="{ regexp('^Merchant', 'Miva Merchant') }" />  - Returns 0
*/
void miva_regexp(mvProgram prog, mvVariableHash parameters, mvVariable returnvalue, void ** unknown) {
	int expr_length, str_length, opts_length;
	const char *expr, *str, *opts;
	char* result;
	int result_length;

	pcre *re = NULL;
	int options = 0, erroffset, rc, offset;
	signed int option_index = 0;
	const char *error;
	int ovector[OVECCOUNT];

	expr = mvVariable_Value(mvVariableHash_Index(parameters, 0), &expr_length);
	str  = mvVariable_Value(mvVariableHash_Index(parameters, 1), &str_length);
	opts = mvVariable_Value(mvVariableHash_Index(parameters, 2), &opts_length);

	// add any options passed for the expression
	if(strlen(opts) > 0){
		for(option_index; option_index < strlen(opts); option_index++){
			switch(opts[option_index]){
				/* Perl compatible options */
				case 'i':	options |= PCRE_CASELESS;		break;
				case 'm':	options |= PCRE_MULTILINE;		break;
				case 's':	options |= PCRE_DOTALL;			break;
				case 'x':	options |= PCRE_EXTENDED;		break;
		
				/* PCRE specific options */
				case 'A':	options |= PCRE_ANCHORED;		break;
				case 'D':	options |= PCRE_DOLLAR_ENDONLY; break;
				//case 'S':	do_study  = 1;					break;
				case 'U':	options |= PCRE_UNGREEDY;		break;
				case 'X':	options |= PCRE_EXTRA;			break;
				case 'u':	options |= PCRE_UTF8;			break;
				case ' ':
				case '\n':
					break;

			}
		}
	}

	re = pcre_compile(expr, options, &error, &erroffset, NULL); 

	if (re == NULL) {
		mvVariable_SetValue_Integer( returnvalue, -1);
		return;
	}

	rc = pcre_exec(re, NULL, str, strlen(str), 0,  0, ovector,  OVECCOUNT);
	
	if (rc < 0) {
		switch(rc) {
			case PCRE_ERROR_NOMATCH: 
				mvVariable_SetValue_Integer( returnvalue, 0); 
				break;
			default: 
				mvVariable_SetValue_Integer( returnvalue, -1); 
				break;
		}
		pcre_free(re);  
		return;
	}

	offset = ovector[0]+1;

	mvVariable_SetValue_Integer( returnvalue,  offset);

	pcre_free(re);
}

/**
* Hook the functions into Miva
*/ 
EXPORT MV_EL_Function_List *miva_function_table() {
	static MV_EL_Function exported_functions[] = {
		{ "regexp", 6, 3, miva_regexp_parameters, miva_regexp },
		{ 0 , 0 , 0, 0 , 0 }
	};

	static MV_EL_Function_List list = { MV_EL_FUNCTION_VERSION, exported_functions };

	return &list;
}


#ifdef __cplusplus
}
#endif