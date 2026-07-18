/* Tokenizer and parser for Saíra Game Data files (.sgd) */
#include "saira.h"

#define WHITESPACE(val) (val == '\n' || val == ' ' || val == '\t')
#define LABEL(val) (val == '[' || val == ']')
#define COMMENT   '#'
#define EQUAL     '='
#define TOKEN_MAX_LEN 256

typedef enum{
	OLABEL = 0,
	CLABEL,
	VALUE,
	ASSIGMENT,
	INVALID
}token_type;

struct token{
	const char *val;
	uint8_t valCount;
	token_type type;
};

static inline bool isTokenTextValid(char *text){
	uint8_t i = 0;
	while(text[i] != '\0'){
		if(!(text[i] >= 'a' && text[i] <= 'z') && !(text[i] >= '0' && text[i] <= '9')){
			return false;
		}
		i++;
	}

	return true;
}

void DA_APPEND2(token t, tokens *array){
	if(array->count == 0){
		array->items = (token*) malloc(sizeof(token));
		assert(array->items != NULL);
	}else if(array->capacity < sizeof(token) * (array->count+1)){
		array->items = (token*) realloc(array->items, sizeof(token) * (array->count + 1));
	}
	
	array->items[array->count] = t;
	array->count++;
	array->capacity = sizeof(token) * array->count;
}

typedef enum{
	FETCHING,
	IGNORING,
	APPENDING,
	APPENDING_STRING
}status;
static tokens tokensArray  = {};
static tokens tokensArrayTest  = {};
static status tokenStatus = FETCHING;
token currentToken = {};

tokens *GetAllTokens(const char *raw){
	tokenStatus = FETCHING;
	if(tokensArray.count > 0){
		free(tokensArray.items);
		tokensArray = (tokens){0};
	}
	size_t  cursor          = 0;
	uint8_t tokenTextSize   = 0;
	uint8_t tokenTextCursor = 0;
	char tokenText[TOKEN_MAX_LEN]  = {0};
	while(raw[cursor] != '\0'){
		char c = raw[cursor++];
		if(WHITESPACE(c) && tokenStatus == FETCHING) continue;
		if(c != '\n' && tokenStatus == IGNORING) continue;

		if(c == COMMENT  && tokenStatus == FETCHING){
			tokenStatus = IGNORING;
			continue;
		}
		if(c == '\n' && tokenStatus == IGNORING){
		  	tokenStatus = FETCHING;	
			continue;
		}
		if(tokenStatus == FETCHING){
			if(c == '"') tokenStatus = APPENDING_STRING;
			else tokenStatus = APPENDING;
			tokenText[tokenTextCursor] = c;
			tokenTextCursor++;
			tokenTextSize++;
			continue;
		}
		if(tokenStatus == APPENDING_STRING && c == '"') tokenStatus = APPENDING;
		if(tokenStatus == APPENDING && (WHITESPACE(c) || LABEL(c) || c == COMMENT)){
			/* token is build here */
			token_type tokenType;

			if(tokenText[0] == '[' && c == ']'){
				if(tokenText[1] == '/') tokenType = CLABEL;
				else tokenType = OLABEL;
				tokenText[tokenTextCursor] = c;
				tokenTextCursor++;
				tokenTextSize++;
			}else if(tokenText[0] == EQUAL){
				tokenType = ASSIGMENT;
			}else if(isTokenTextValid(tokenText) || tokenText[0] == '"'){
				tokenType = VALUE;
			}else tokenType = INVALID;
			const char *ptr = malloc(sizeof(char) * (tokenTextSize+1));
			ptr = memcpy((void*)ptr, tokenText, tokenTextSize+1);		
			token newToken = {
				.val = ptr,
				.valCount = tokenTextSize,
				.type = tokenType};
			
			DA_APPEND2(newToken, &tokensArrayTest);
			tokenStatus = FETCHING;
			memset(tokenText, '\0', TOKEN_MAX_LEN);
			tokenTextCursor = 0;
			tokenTextSize   = 0;
		}else{
			tokenText[tokenTextCursor] = c;
			tokenTextCursor++;
			tokenTextSize++;
			continue;
		}
			
	}
	
	return &tokensArray;
}

nodes b = {};
nodes ParseTokens(tokens *allTokens){
	return b;
}
