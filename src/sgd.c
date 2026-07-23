// Tokenizer and parser for Saíra Game Data files (.sgd).
// TODO: still very unsafe. A misformatted file may cause segfault without further memory boundary checks.
//
// Use GetAllTokens() to get all tokens from a string and ParseTokens() to parse it.
//	Tokens are allocated at dynamic memory and are freed everytime GetAllToken() is called. Nodes and constants are
// stored in static memory with MAX_NODES and MAX_CONSTANTS restriction. Each node's pointers to children/constants
// are also freed when calling ParseTokens() again.

#include "saira.h"

#define WHITESPACE(val) (val == '\n' || val == ' ' || val == '\t')
#define LABEL(val) (val == '[' || val == ']')
#define COMMENT   '#'
#define EQUAL     '='
#define TOKEN_MAX_LEN 256
#define VERSION "1.0.0" /* using SemVer. Can only parse files at the same major version */
#define OPEN_SIGNATURE  "[SGD]"
#define CLOSE_SIGNATURE "[/SGD]"
#define MAX_NODES 1024
#define MAX_CONSTANTS 1024

typedef enum{
	OLABEL = 0,
	CLABEL,
	VALUE,
	ASSIGMENT,
	INVALID
}token_type;

/* TODO: token line and column */
struct token{
	char *val;
	uint8_t valCount;
	token_type type;
};

static inline bool isTokenTextValid(char *text){
	uint8_t i = 0;
	while(text[i] != '\0'){
		if(text[i] <= 44) return false;
		i++;
	}

	return true;
}

/*TODO: hash table */
ptr_da GetNodes(const char *nodeName, node *searchArray){
	ptr_da resultArray = {0};
	
	for(size_t i = 0; i < MAX_NODES; i++){
		if(searchArray[i].name == 0) break;
		if(strcmp(searchArray[i].name, nodeName) == 0){
			DA_PUSH(&searchArray[i], resultArray);
		}
	}

	return resultArray;
}

const char *GetConstantValue(node *searchNode, const char *constantName){
	for(size_t i = 0; i < searchNode->constantCount; ++i){
		if (strcmp(searchNode->constants[i]->name, constantName) == 0){
			return searchNode->constants[i]->value;
		}
	}

	return NULL;
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
			
			/* a bracket ended the current token instead of whitespace. Must add this bracket to the next token. */
			if(LABEL(c) && (newToken.type != OLABEL && newToken.type != CLABEL))
				cursor--; 
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

static token GetExpectToken(token_type expectedType, tokens *allTokens, size_t *ptr){
	token t = NextToken(allTokens, ptr);
	if(t.type != expectedType){
		t.type = INVALID;
		return t;
	}

	return t;
}
static void ClearTokenName(token *t){
	/* clearing only opening label tokens for now */
	/* TODO: more types of tokens and whitespace the lexer may have let pass */
	for(int i = 1; i < t->valCount; ++i) t->val[i-1] = t->val[i];
	t->val[t->valCount-2]= '\0';
	t->valCount -= 2;
}

void PrintNodeTree(node *root, int tabLevel){
	const char *tabs = "\t\t\t\t\t\t\t\t\t\t";
	if(root->childrenCount == 0){
		TraceLog(LOG_INFO, "%.*s%s;", tabLevel, tabs,root->name);
		return;
	}else{
		TraceLog(LOG_INFO, "%.*s[%s];", tabLevel, tabs,root->name);
		for(size_t i = 0; i < root->childrenCount; ++i){
			PrintNodeTree(root->children[i], tabLevel + 1);
		}
		TraceLog(LOG_INFO, "%.*s[/%s];", tabLevel, tabs,root->name);
	}
}

bool CreateNode(token nodeToken, size_t *arrayPtr, int parent, node nodeArray[]){
	if(*arrayPtr >= MAX_NODES){
		TraceLog(LOG_ERROR, "Too many nodes!");
		return false; 
	}

	node newNode = {0};
	ClearTokenName(&nodeToken);
	newNode.name = nodeToken.val; /* when GetAllTokens() is called this may be gone. */
	if(parent == -1){ /* root node */
		nodeArray[*arrayPtr] = newNode;
		*arrayPtr = *arrayPtr + 1;
		return true;
	}

	nodeArray[*arrayPtr] = newNode;

	if(nodeArray[parent].childrenCount == 0){
		/* TODO: I think I can use realloc here just fine and only alloc enough memory */
		nodeArray[parent].children = (node**) malloc(sizeof(void*) * MAX_NODES);
		nodeArray[parent].children[nodeArray[parent].childrenCount] = &nodeArray[*arrayPtr];
		nodeArray[parent].childrenCount += 1;
	}else{
		nodeArray[parent].children[nodeArray[parent].childrenCount] = &nodeArray[*arrayPtr];
		nodeArray[parent].childrenCount += 1;
	}

	*arrayPtr = *arrayPtr + 1;

	return true;
}

node *ParseTokens(tokens *allTokens){
	static node allNodes[MAX_NODES]             = {0};
	static constant allConstants[MAX_CONSTANTS] = {0};
	static size_t nodePtr                       = 0;
	static size_t constantPtr                   = 0;
	static size_t tokenPtr                      = 0; 

	if(nodePtr > 0){
		for(size_t i = 0; i < nodePtr-1; i++){
			if(allNodes[i].childrenCount > 0) free(allNodes[i].children);
			if(allNodes[i].constantCount > 0) free(allNodes[i].constants);
		}
		nodePtr = 0;
		constantPtr = 0;
		tokenPtr = 0;
		memset(allNodes, 0, sizeof(allNodes));
		memset(allConstants, 0, sizeof(allConstants));
	}

	bool print = false;
	/* signature validation */
	if(strcmp(NextToken(allTokens, &tokenPtr).val,OPEN_SIGNATURE) == 0){
		char *fileVersion = "0.0.0";
		token t = {0};
		/* TODO: signature parsing errors */
		for(;;){
			t = NextToken(allTokens, &tokenPtr);
			if(strcmp(t.val, CLOSE_SIGNATURE) == 0 || t.type == INVALID) break;
			if(strcmp(t.val, "version") == 0){
				NextToken(allTokens, &tokenPtr);
				fileVersion = (char*) NextToken(allTokens, &tokenPtr).val;
			}else if(strcmp(t.val, "printme") == 0){
				NextToken(allTokens, &tokenPtr);
				print = (strcmp(NextToken(allTokens, &tokenPtr).val,"true") == 0);
			}
		} 
		
		if(strcmp(t.val, CLOSE_SIGNATURE) != 0){
			TraceLog(LOG_ERROR, "Signature not closed.");
			return NULL;
		}
		if(fileVersion[0] != VERSION[0]){
			TraceLog(LOG_ERROR, "Wrong or missing version. Wanted  <%s>, found <%s>.", VERSION, fileVersion);
			return NULL;
		}
	}else{
		TraceLog(LOG_WARNING, "No token signature found %s", OPEN_SIGNATURE);
		return NULL;
	}
	/* end of signature validation */
	
	token rootToken = NextToken(allTokens, &tokenPtr);
	if(rootToken.type != OLABEL){
		TraceLog(LOG_ERROR, "No root found after signature!");
		return NULL;
	}
	
	typedef struct{
		size_t *items;
		size_t count;
		size_t capacity;
	}stack;
	
	static stack labelStack            = {0};
	size_t depth                       = 0;

	if(!CreateNode(rootToken, &nodePtr, -1, allNodes)){
		TraceLog(LOG_ERROR, "Failure during creation of root node");
		return NULL;
	}
	DA_PUSH(nodePtr-1, labelStack);

	token t;
	while((t = NextToken(allTokens, &tokenPtr)).type != INVALID){
		if (t.type == OLABEL) {
			size_t parentIndex = labelStack.items[labelStack.count-1];
			if(!CreateNode(t, &nodePtr, parentIndex, allNodes)){
				TraceLog(LOG_ERROR, "Failure during creation of node");
				return NULL;
			}
			depth++;
			DA_PUSH(nodePtr-1, labelStack);
		}else if (t.type == CLABEL && !(tokenPtr >= allTokens->count)){
			depth--;
			if(depth > 1024 ){
				TraceLog(LOG_ERROR, "Unbalanced or overflow labels");
				return NULL;
			}
			DA_POP(labelStack);
		}else if(t.type == VALUE){
			const char *constantName = t.val;
			if((t = GetExpectToken(ASSIGMENT, allTokens, &tokenPtr)).type != INVALID){
				if((t = GetExpectToken(VALUE, allTokens, &tokenPtr)).type != INVALID){
					const char *constantVal = t.val;
					constant c = {
						.name   = constantName,
						.value  = constantVal
					};
					node *currentNode = &allNodes[nodePtr-1];
					if (currentNode->constantCount == 0){
						/* TODO: magic number */
						currentNode->constants = (void*) malloc(256 * sizeof(void*));
						assert(currentNode->constants != NULL && "not enough memory");
					}
					allConstants[constantPtr] = c;
					currentNode->constants[currentNode->constantCount] = &allConstants[constantPtr];
					currentNode->constantCount++;
					constantPtr++;
					continue; 
				}
			}
			TraceLog(LOG_ERROR, "wrong definition of constant <%s>",t.val); 
		}else continue;
	}
	if(print){
		TraceLog(LOG_DEBUG, "--- printing node tree for .sgd file with root: <%s> ---", allNodes[0].name);
		PrintNodeTree(&allNodes[0], 0);
	}
	free(labelStack.items);
	return allNodes;
}
