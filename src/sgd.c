/* Tokenizer and parser for Saíra Game Data files (.sgd) */
#include "saira.h"

#define WHITESPACE(val) (val == '\n' || val == ' ' || val == '\t')
#define LABEL(val) (val == '[' || val == ']')
#define COMMENT   '#'
#define EQUAL     '='
#define TOKEN_MAX_LEN 256
#define VERSION "1.0.0" /* using SemVer. Can only parse files at the same major version */
#define OPEN_SIGNATURE  "[SGD]"
#define CLOSE_SIGNATURE "[/SGD]"

typedef enum{
	OLABEL = 0,
	CLABEL,
	VALUE,
	ASSIGMENT,
	INVALID
}token_type;

struct token{
	char *val;
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

typedef enum{
	FETCHING,
	IGNORING,
	APPENDING,
	APPENDING_STRING
}status;
static status tokenStatus = FETCHING;
token currentToken = {};

tokens *GetAllTokens(const char *raw){
	static tokens tokensArray  = {};
	tokenStatus = FETCHING;
	if(tokensArray.count > 0){
		/*TODO: also free each token.val you allocated with *ptr */
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
			char *ptr = malloc(sizeof(char) * (tokenTextSize+1));
			ptr = memcpy((void*)ptr, tokenText, tokenTextSize+1);		
			token newToken = {
				.val = ptr,
				.valCount = tokenTextSize,
				.type = tokenType};
			
			DA_APPEND(newToken, tokensArray);
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

static token NextToken(tokens *allTokens, size_t *ptr){
	token t = (token){};
	if(*ptr >= allTokens->count){
		*ptr = *ptr + 1;
		t.type = INVALID;
		return t;
	}
	
	t = allTokens->items[*ptr];
	*ptr = *ptr + 1;
	return  t;
}

static void ClearTokenName(token *t){
	/* clearing only opening label tokens for now */
	/* TODO: more types of tokens and whitespace the lexer may have let pass */
	for(int i = 1; i < t->valCount; ++i) t->val[i-1] = t->val[i];
	t->val[t->valCount-2]= '\0';
	t->valCount -= 2;
}

/*
struct node{
	const char *name;
	size_t childrenCount;
	size_t constantCount;
	constant *constants;
	node *children;
};*/

node CreateNode(token nodeToken, node *parentNode, nodes *nodeArray){
	node newNode = {0};
	ClearTokenName(&nodeToken);
	newNode.name = nodeToken.val; /* when GetAllTokens() is called this may be gone. */
	if(parentNode == NULL){
		/* create a root node */
		newNode.children = NULL;
		DA_APPEND(newNode, (*nodeArray));
		return newNode;
	}

	if(parentNode->childrenCount == 0){
		parentNode->children = (node*) malloc(sizeof(newNode));
	}else{
		parentNode->children = (node*) realloc(parentNode->children, sizeof(newNode) * (parentNode->childrenCount + 1));
	}
	parentNode->children[parentNode->childrenCount] = newNode;
	parentNode->childrenCount+=1;
	DA_APPEND(newNode, (*nodeArray));

	return newNode;
}

nodes ParseTokens(tokens *allTokens){
	static nodes nodesArray = {};
	if(nodesArray.count > 0){
		free(nodesArray.items);
		nodesArray = (nodes){0};
	}
	size_t ptr = 0;
	/* signature validation */
	if(strcmp(NextToken(allTokens, &ptr).val,OPEN_SIGNATURE) == 0){
		char *fileVersion = "0.0.0";
		token t = {0};
		for(;;){
			t = NextToken(allTokens, &ptr);
			if(strcmp(t.val, CLOSE_SIGNATURE) == 0 || t.type == INVALID) break;
			if(strcmp(t.val, "version") == 0){
				NextToken(allTokens, &ptr);
				fileVersion = (char*) NextToken(allTokens, &ptr).val;
			} 
		} 
		
		if(strcmp(t.val, CLOSE_SIGNATURE) != 0){
			TraceLog(LOG_ERROR, "Signature not closed.");
			return nodesArray;
		}
		if(fileVersion[0] != VERSION[0]){
			TraceLog(LOG_ERROR, "Wrong or missing version. Wanted  <%s>, found <%s>.", VERSION, fileVersion);
			return nodesArray;
		}
	}else{
		TraceLog(LOG_WARNING, "No token signature found %s", OPEN_SIGNATURE);
		return nodesArray;
	}
	/* end of signature validation */
	
	token rootToken = NextToken(allTokens, &ptr);
	if(rootToken.type != OLABEL){
		TraceLog(LOG_ERROR, "No root found after signature!");
		return nodesArray;
	}
	node rootNode = CreateNode(rootToken, NULL, &nodesArray);
	long long lastNode = nodesArray.count-1;

	token t;
	while((t = NextToken(allTokens, &ptr)).type != INVALID){
		if (t.type == OLABEL) {
			CreateNode(t, &nodesArray.items[lastNode], &nodesArray);	
			lastNode = nodesArray.count-1;
		}else if (t.type == CLABEL && !(ptr >= allTokens->count)){
			lastNode--;
			if(lastNode < 0 ){
				TraceLog(LOG_ERROR, "Unbalanced labels");
				free(nodesArray.items);
				nodesArray = (nodes){0};
				return nodesArray;
			}
		}

	}
	nodesArray = (nodes){0};
	return nodesArray;
}
