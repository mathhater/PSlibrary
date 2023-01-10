#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PLUS 1
#define MINUS -1
#define PI 3.14159265358979323846

struct Complex{
    double r, i;
};

struct BigInt {
	char *digits;
	int size, sign;
};

struct BigFloat {
	char *decimal, *integer;
	int sizeDecimal, sizeInteger;
};

struct Complex addComplex(struct Complex a, struct Complex b){
    struct Complex res;
    res.r=a.r+b.r;
    res.i=a.i+b.i;
    return res;
}

struct Complex subtractComplex(struct Complex a, struct Complex b){
    struct Complex res;
    res.r=a.r-b.r;
    res.i=a.i-b.i;
    return res;
}

struct Complex multiplyComplex(struct Complex a, struct Complex b){
    struct Complex res;
    res.r=a.r*b.r-a.i*b.i;
    res.i=a.r*b.i+a.i*b.r;
    return res;
}

struct Complex divideComplex(struct Complex a, struct Complex b){
    struct Complex res;
    res.r=(a.r*b.r+a.i*b.i)/(b.r*b.r+b.i*b.i);
    res.i=(-a.r*b.i+a.i*b.r)/(b.r*b.r+b.i*b.i);
    return res;
}

void fft(struct Complex a[], int n, int inv){
    int i, j, k, bit;
    struct Complex w, th, tmp;
    double x;

    for(i=1, j=0; i<n; ++i){
        bit=n>>1;
        while(!((j^=bit) & bit)){
            bit>>=1;
        }
        if(i<j){
            tmp=a[i];
            a[i]=a[j];
            a[j]=tmp;
        }
    }

    for(i=1; i<n; i<<=1){
        x=(inv)?PI/i:-PI/i;
        w.r=cos(x);
        w.i=sin(x);
        for(j=0; j<n; j+=(i<<1)){
            th.r=1;
            th.i=0;
            for(k=0; k<i; ++k){
                tmp=multiplyComplex(a[i+j+k], th);
                a[i+j+k]=subtractComplex(a[j+k], tmp);
                a[j+k]=addComplex(a[j+k], tmp);
                th=multiplyComplex(th, w);
            }
        }
    }

    if(inv){
        tmp.r=n;
        tmp.i=0;
        for(i=0; i<n; ++i){
            a[i]=divideComplex(a[i], tmp);
        }
    }
}

void str2complex(struct Complex comp[], char str[], int len, int x){
    int i, j, tmp, flag=(len%x>0);

    for(i=0; i<len/x+flag; ++i){
        tmp=0;
        for(j=x-1; j>=0; --j){
            if(len-(i*x+j)-1<0 || len-(i*x+j)-1>=len+2){
                continue;
            }
            if(!str[len-(i*x+j)-1]){
                continue;
            }
            tmp=(tmp*10)+str[len-(i*x+j)-1]-'0';
        }
        comp[i].r=tmp;
    }
}

struct BigInt *createBigInt(char str[]){
	struct BigInt *bigint=(struct BigInt *)malloc(sizeof(struct BigInt));
	int len=strlen(str);
	bigint->digits=(char *)malloc(sizeof(char)*(len+1));
	if(len && str[0]=='-'){
		bigint->sign=MINUS;
		strcpy(bigint->digits, str+1);
		bigint->size=len-1;
	}

	int idx=0;
	while(str[idx]=='0'){
		++idx;
	}

	if(str[0]==0){
		char tmp[2]="0";
		bigint->sign=PLUS;
		strcpy(bigint->digits, tmp);
		bigint->size=1;
	}
	else{
		bigint->sign=PLUS;
		strcpy(bigint->digits, str);
		bigint->size=len;
	}
	return bigint;
}

void deleteBigInt(struct BigInt *bigint){
	free(bigint->digits);
	free(bigint);
}

int cmpDigitsBigInt(struct BigInt *a, struct BigInt *b){
	if(a->size==b->size){
		for(int i=0; i<a->size; ++i){
			if(a->digits[i]!=b->digits[i]){
				return a->digits[i]>b->digits[i] ? 1 : -1;
			}
		}
		return 0;
	}
	return a->size>b->size ? 1 : -1;
}

int cmpBigInt(struct BigInt *a, struct BigInt *b){
	if(a->sign==PLUS){
		if(b->sign==PLUS){
			return cmpDigitsBigInt(a, b);
		}
		return 1;
	}
	else{
		if(b->sign==PLUS){
			return -1;
		}
		return -cmpDigitsBigInt(a, b);
	}
}

void printBigInt(struct BigInt *a){
	if(a->sign==MINUS){
		printf("-");
	}
	printf("%s", a->digits);
}

struct BigInt *_addBigInt(struct BigInt *a, struct BigInt *b){
	int flag=0, idx=0, idxa=a->size, idxb=b->size;
	char *str=(char *)malloc(sizeof(char)*(idxa>idxb ? idxa+2 : idxb+2));
	while(idxa && idxb){
		int tmp=(a->digits[idxa-1]-'0')+(b->digits[idxb-1]-'0')+flag;
		str[idx++]=tmp%10+'0';
		flag=tmp/10;
		--idxa;
		--idxb;
	}
	while(idxa){
		int tmp=(a->digits[idxa-1]-'0')+flag;
		str[idx++]=tmp%10+'0';
		flag=tmp/10;
		--idxa;
	}
	while(idxb){
		int tmp=(b->digits[idxb-1]-'0')+flag;
		str[idx++]=tmp%10+'0';
		flag=tmp/10;
		--idxb;
	}
	if(flag){
		str[idx++]=flag+'0';
	}

	for(int i=0; i<idx/2; ++i){
		char ch=str[i];
		str[i]=str[idx-1-i];
		str[idx-1-i]=ch;
	}
	str[idx]=0;
	struct BigInt *res=createBigInt(str);
	free(str);
	return res;
}

struct BigInt *_subtractBigInt(struct BigInt *a, struct BigInt *b){
	int flag=0, idx=0, idxa=a->size, idxb=b->size;
	char *str=(char *)malloc(sizeof(char)*(idxa>idxb ? idxa+1 : idxb+1));
	while(idxa && idxb){
		int tmp=(a->digits[idxa-1]-'0')-(b->digits[idxb-1]-'0')-flag;
		flag=0;
		if(tmp<0){
			tmp+=10;
			flag=1;
		}
		str[idx++]=tmp+'0';
		--idxa;
		--idxb;
	}
	while(idxa){
		int tmp=(a->digits[idxa-1]-'0')-flag;
		flag=0;
		if(tmp<0){
			tmp+=10;
			flag=1;
		}
		str[idx++]=tmp+'0';
		--idxa;
	}

	for(int i=0; i<idx/2; ++i){
		char ch=str[i];
		str[i]=str[idx-1-i];
		str[idx-1-i]=ch;
	}
	str[idx]=0;
	int start=0;
	for(; start<idx; ++start){
		if(str[start]!='0'){
			break;
		}
	}
	struct BigInt *res=createBigInt(str+start);
	free(str);
	return res;
}

struct BigInt *addBigInt(struct BigInt *a, struct BigInt *b){
	if(a->sign==b->sign){
		struct BigInt *res=_addBigInt(a, b);
		res->sign=a->sign;
		return res;
	}
	else{
		int flag=cmpDigitsBigInt(a, b);
		if(flag){
			if(flag>0){
				struct BigInt *res=_subtractBigInt(a, b);
				res->sign=a->sign;
				return res;
			}
			else{
				struct BigInt *res=_subtractBigInt(b, a);
				res->sign=b->sign;
				return res;
			}
		}
		else{
			char str[2]="0";
			struct BigInt *res=createBigInt(str);
			return res;
		}
	}
}

struct BigInt *subtractBigInt(struct BigInt *a, struct BigInt *b){
	struct BigInt *res;
	if(a->sign==PLUS && b->sign==PLUS){
		int flag=cmpDigitsBigInt(a, b);
		if(flag>0){
			res=_subtractBigInt(a, b);
			res->sign=PLUS;
			return res;
		}
		else if(flag<0){
			res=_subtractBigInt(b, a);
			res->sign=MINUS;
			return res;
		}
		else{
			char str[2]="0";
			res=createBigInt(str);
			return res;
		}
	}
	else if(a->sign==PLUS && b->sign==MINUS){
		res=_addBigInt(a, b);
		res->sign=PLUS;
		return res;
	}
	else if(a->sign==MINUS && b->sign==PLUS){
		res=_addBigInt(a, b);
		res->sign=MINUS;
		return res;
	}
	else{
		int flag=cmpDigitsBigInt(a, b);
		if(flag>0){
			res=_subtractBigInt(a, b);
			res->sign=MINUS;
			return res;
		}
		else if(flag<0){
			res=_subtractBigInt(b, a);
			res->sign=PLUS;
			return res;
		}
		else{
			char str[2]="0";
			res=createBigInt(str);
			return res;
		}
	}
}

void _multiplyBigInt_fft(struct Complex res[], struct Complex a[], int len_a, struct Complex b[], int len_b){
    int i, n=1;

    while(n<len_a+1 || n<len_b+1){
        n<<=1;
    }
    n<<=1;

    // DFT a and b with FFT
    fft(a, n, 0);
    fft(b, n, 0);

    // convolution
    for(i=0; i<n; ++i){
        res[i]=multiplyComplex(a[i], b[i]);
    }
    
    // IDFT
    fft(res, n, 1);
    for(i=0; i<n; ++i){
        res[i].r=round(res[i].r);
        res[i].i=round(res[i].i);
    }
}

struct BigInt *_multiplyBigInt1(struct BigInt *a, struct BigInt *b){
	// normal
	if(a->size<b->size){
		return _multiplyBigInt1(b, a);
	}

	char zero[2]="0", *tmpstr=(char *)malloc(sizeof(char)*(a->size+b->size+1));
	struct BigInt *tmp1, *tmp2, *res=createBigInt(zero);

	for(int i=b->size-1; i>=0; --i){
		int vval=0, val=b->digits[i]-'0';
		int idx=0;

		if(val==0){
			continue;
		}

		for(int j=a->size-1; j>=0; --j){
			vval+=(int)(a->digits[j]-'0')*val;
			tmpstr[idx++]=vval%10+'0';
			vval/=10;
		}
		while(vval){
			tmpstr[idx++]=vval%10+'0';
			vval/=10;
		}

		for(int j=0; j<idx/2; ++j){
			char ch=tmpstr[j];
			tmpstr[j]=tmpstr[idx-1-j];
			tmpstr[idx-1-j]=ch;
		}
		for(int j=0; j<b->size-1-i; ++j){
			tmpstr[idx++]='0';
		}
		tmpstr[idx]=0;

		tmp1=createBigInt(tmpstr);
		tmp2=res;
		res=addBigInt(tmp1, tmp2);
		deleteBigInt(tmp1);
		deleteBigInt(tmp2);
	}
	free(tmpstr);

	return res;
}

// too slow
/*

struct BigInt *karatsuba(struct BigInt *a, struct BigInt *b){
	// karatsuba
	if(a->size<b->size){
		return karatsuba(b, a);
	}

	if(a->size==1 && b->size==1){
		char str[3];
		int c=(a->digits[0]-'0')*(b->digits[0]-'0');
		if(c>=10){
			str[0]=c/10+'0';
			str[1]=c%10+'0';
			str[2]=0;
		}
		else{
			str[0]=c+'0';
			str[1]=0;
		}
		return createBigInt(str);
	}

	// if(a->size<=100){
	// 	return _multiplyBigInt1(a, b);
	// }

	char ch;
	struct BigInt *leftA, *rightA, *leftB, *rightB;
	int p=(a->size+((a->size & 1) ? 1 : 0))/2;

	int q=a->size-p;
	ch=a->digits[q];
	a->digits[q]=0;
	leftA=createBigInt(a->digits);
	a->digits[q]=ch;
	rightA=createBigInt(a->digits+q);

	if(b->size<=p){
		char str[2]="0";
		leftB=createBigInt(str);
		rightB=createBigInt(b->digits);
	}
	else{
		int q=b->size-p;
		ch=b->digits[q];
		b->digits[q]=0;
		leftB=createBigInt(b->digits);
		b->digits[q]=ch;
		rightB=createBigInt(b->digits+q);
	}

	struct BigInt *z0=karatsuba(leftA, leftB);
	struct BigInt *AA=addBigInt(leftA, rightA);
	struct BigInt *BB=addBigInt(leftB, rightB);
	struct BigInt *z1=karatsuba(AA, BB);
	struct BigInt *z2=karatsuba(rightA, rightB);

	deleteBigInt(leftA);
	deleteBigInt(rightA);
	deleteBigInt(leftB);
	deleteBigInt(rightB);
	deleteBigInt(AA);
	deleteBigInt(BB);

	struct BigInt *z10=subtractBigInt(z1, z0);
	struct BigInt *z=subtractBigInt(z10, z2);

	if(z0->digits[0]!='0'){
		char *str=(char *)malloc(sizeof(char)*(z0->size+p*2+1));
		strcpy(str, z0->digits);
		for(int i=0; i<p*2; ++i){
			str[z0->size+i]='0';
		}
		str[z0->size+p*2]=0;
		z0->size+=p*2;
		z0->digits=(char *)realloc(z0->digits, z0->size+1);
		strcpy(z0->digits, str);
		free(str);
	}

	if(z->digits[0]!='0'){
		char *str=(char *)malloc(sizeof(char)*(z->size+p+1));
		strcpy(str, z->digits);
		for(int i=0; i<p; ++i){
			str[z->size+i]='0';
		}
		str[z->size+p]=0;
		z->size+=p;
		z->digits=(char *)realloc(z->digits, z->size+1);
		strcpy(z->digits, str);
		free(str);
	}

	struct BigInt *tmp=addBigInt(z, z0);
	struct BigInt *res=addBigInt(z2, tmp);
	deleteBigInt(z0);
	deleteBigInt(z1);
	deleteBigInt(z2);
	deleteBigInt(z10);
	deleteBigInt(z);
	deleteBigInt(tmp);

	return res;
}

*/

struct BigInt *_multiplyBigInt2(struct BigInt *a, struct BigInt *b){
	// fft
	struct BigInt *res;
	int lencpx=1;
	while(lencpx<=a->size || lencpx<=b->size){
		lencpx*=2;
	}
	lencpx*=2;

	struct Complex *aa, *bb, *cc;
	int *ans, div=2;
	aa=(struct Complex *)calloc(lencpx, sizeof(struct Complex));
	bb=(struct Complex *)calloc(lencpx, sizeof(struct Complex));
	cc=(struct Complex *)calloc(lencpx, sizeof(struct Complex));
	ans=(int *)calloc(a->size+b->size+1, sizeof(int));
	str2complex(aa, a->digits, a->size, div);
	str2complex(bb, b->digits, b->size, div);

	_multiplyBigInt_fft(cc, aa, a->size/div+(a->size%div>0), bb, b->size/div+(b->size%div>0));

	for(int i=0; i<lencpx; ++i){
		long long tmp=(long long)cc[i].r;
		int j=0;
		while(tmp && i*div+j<a->size+b->size){
			ans[i*div+j]+=(int)(tmp%10LL);
			tmp/=10LL;
			++j;
		}
	}
	for(int i=0; i<a->size+b->size-1; ++i){
		if(ans[i]>9){
			ans[i+1]+=ans[i]/10;
			ans[i]%=10;
		}
	}

	free(aa);
	free(bb);
	free(cc);

	int idx=0;
	char *str=(char *)calloc(a->size+b->size+1, sizeof(char));

	if(ans[a->size+b->size-1]!=0){
		str[idx++]=ans[a->size+b->size-1]+'0';
	}
	for(int i=a->size+b->size-2; i>=0; --i){
		str[idx++]=ans[i]+'0';
	}
	str[idx]=0;

	res=createBigInt(str);

	free(ans);
	free(str);

	return res;
}

struct BigInt *multiplyBigInt(struct BigInt *a, struct BigInt *b){
	if(a->digits[0]=='0' || b->digits[0]=='0'){
		char tmp[2]="0";
		return createBigInt(tmp);
	}

	struct BigInt *res;

	if(a->size<=100 || b->size<=100){
		res=_multiplyBigInt1(a, b);
	}
	else{
		res=_multiplyBigInt2(a, b);
	}

	if(a->sign==b->sign){
		res->sign=PLUS;
	}
	else{
		res->sign=MINUS;
	}

	return res;
}

struct BigInt *_divideBigInt1(struct BigInt *a, struct BigInt *b){
	char ch, str[2]="0", _zero[2]="0", _ten[3]="10";
	struct BigInt *zero=createBigInt(_zero), *ten=createBigInt(_ten);
	struct BigInt *tmp, *ttmp, *r=createBigInt(_zero), *q=createBigInt(_zero);

	for(int i=0; i<=a->size; ++i){
		int cnt=0;
		while(cmpDigitsBigInt(r, b)>=0){
			tmp=subtractBigInt(r, b);
			deleteBigInt(r);
			r=tmp;
			++cnt;
		}
		tmp=multiplyBigInt(q, ten);
		deleteBigInt(q);
		q=tmp;

		str[0]=cnt+'0';
		str[1]=0;
		tmp=createBigInt(str);
		ttmp=addBigInt(q, tmp);
		deleteBigInt(tmp);
		deleteBigInt(q);
		q=ttmp;

		if(i!=a->size){
			tmp=multiplyBigInt(r, ten);
			deleteBigInt(r);
			r=tmp;

			ch=a->digits[i+1];
			a->digits[i+1]=0;
			tmp=createBigInt(a->digits+i);
			a->digits[i+1]=ch;
			ttmp=addBigInt(r, tmp);
			deleteBigInt(tmp);
			deleteBigInt(r);
			r=ttmp;
		}
	}

	deleteBigInt(zero);
	deleteBigInt(ten);
	deleteBigInt(r);
	return q;
}

struct BigInt *divideBigInt(struct BigInt *a, struct BigInt *b){
	if(b->digits[0]=='0'){
		printf("do not divide by 0\n");
		exit(1);
	}
	if(a->digits[0]=='0'){
		char tmp[2]="0";
		return createBigInt(tmp);
	}

	int asign=a->sign, bsign=b->sign;
	a->sign=b->sign=PLUS;
	struct BigInt *q=_divideBigInt1(a, b);
	if(asign==bsign){
		q->sign=PLUS;
	}
	else{
		q->sign=MINUS;
	}

	return q;
}

struct BigInt *modularBigInt(struct BigInt *a, struct BigInt *b){
	if(b->digits[0]=='0'){
		printf("do not divide by 0\n");
		exit(1);
	}
	if(a->digits[0]=='0'){
		char tmp[2]="0";
		return createBigInt(tmp);
	}

	int asign=a->sign, bsign=b->sign;
	a->sign=b->sign=PLUS;
	struct BigInt *q=divideBigInt(a, b);
	struct BigInt *qb=multiplyBigInt(q, b);
	struct BigInt *r=subtractBigInt(a, qb);
	r->sign=asign;

	return r;
}

int main(void){
	char A[300001], B[300001];

	scanf("%s %s", A, B);

	struct BigInt *a=createBigInt(A);
	struct BigInt *b=createBigInt(B);
	struct BigInt *c=divideBigInt(a, b);
	struct BigInt *d=modularBigInt(a, b);
	printf("c: ");
	printBigInt(c);
	printf("\n");
	printf("d: ");
	printBigInt(d);
	printf("\n");

	deleteBigInt(a);
	deleteBigInt(b);
	deleteBigInt(c);

	return 0;
}