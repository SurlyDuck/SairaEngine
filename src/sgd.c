/* Tokenizer and parser for Saíra Game Data files (.sgd) */
#include "saira.h"

typedef enum{
	VARIABLE = 0,
	SEGMENT
}token_type;

typedef struct token token;
struct token{
	const char *val;
	size_t childrenCount;
	token_type type;
	token **children;
};

struct tokens{
	token *items;
	size_t count;
	size_t capacity;
};


tokens a = {};
tokens *GetAllTokens(const char **raw){
	return &a;
}

nodes b = {};
nodes ParseTokens(tokens *allTokens){
	return b;
}
