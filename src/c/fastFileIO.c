#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getString(char *result, char *input, int *index){
	int resIdx=0;
	while(input[*index]!='\n' && input[*index]!=' ' && input[*index]!='\0'){
		result[resIdx++]=input[*index];
		++(*index);
	}
	++(*index);
	result[resIdx]=0;
}

long long getInt(char *input, int *index){
	long long result=0;
	int flag=0;
	if(input[*index]=='-'){
		++(*index);
		flag=1;
	}
	while(input[*index]!='\n' && input[*index]!=' ' && input[*index]!='\0'){
		result*=10;
		result+=input[*index]-'0';
		++(*index);
	}
	++(*index);
	return flag ? -result : result;
}

void setString(char *output, int *outputSize, char *input, int len){
	strcpy(output+(*outputSize), input);
	(*outputSize)+=len;
}

void setInt(char *output, int *index, long long x){
	char str[30];
	int idx=0, flag=0;
	if(x<0){
		x=-x;
		flag=1;
	}
	if(x==0){
		str[idx++]='0';
	}
	else{
		while(x){
			str[idx++]=x%10+'0';
			x/=10;
		}
		if(flag){
			str[idx++]='-';
		}
	}
	while(idx--){
		output[(*index)++]=str[idx];
	}
}

int main(void){
	char *inBuffer, *outBuffer;
	int inIndex=0, outIndex=0;

	FILE *in=fopen("input.txt", "r");
	fseek(in, 0, SEEK_END);
	int inSize=ftell(in);
	inBuffer=(char *)malloc(sizeof(char)*(inSize+inSize/50));
	/*
		Sometimes, when the buffer size is set as the return value of the ftell function,
		there is a problem that the entire file cannot be read, so I set it to a larger value.
	*/
	fseek(in, 0, SEEK_SET);
	inSize=fread(inBuffer, 1, inSize+inSize/50, in);
	fclose(in);

	outBuffer=(char *)malloc(sizeof(char)*(inSize+inSize/50));

    int T=getInt(inBuffer, &inIndex);
    for(int test_case=1; test_case<=T; ++test_case){
    }
	outBuffer[outIndex]=0;
	FILE *out=fopen("output.txt", "w");
	fwrite(outBuffer, 1, outIndex, out);
	fclose(out);

	free(inBuffer);
	free(outBuffer);

    return 0;
}