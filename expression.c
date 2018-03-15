#include "expression.h"

t_symcle tabcle[NBCLE] = {
    { "+", B1 }, { "-", B2 }, { "*", B3 }, { "/", B4 }, { "min", B5 }, { "max", B6 },
    { "sqrt", U1 }, { "exp", U2 }, { "ln", U3 }, { "abs", U4 }, { "floor", U5 }, { "ceil", U6 }
};

void free_expr(expr e)
{
	if(e!=NULL)
	{
		free_expr(e->fd);
		free_expr(e->fg);

		free(e);
	}
}

void free_mem(char ** str)
{
	int i=0;
	while(str[i]!=NULL)
	{
		free(str[i]);
		i++;
	}
	free(str[i]);
	free(str);
	return;
}

int strtocle(char* cle)
{
    unsigned int i;
    for (i=0; i < NBCLE; i++) 
    {
        t_symcle *sym = &tabcle[i];
        if (strcmp(sym->cle, cle) == 0)
            return sym->val;
    }
    return A1;
}

int return_nb(char* str, double* a)
{
	char r;
	int size=strlen(str);	
	int err=0;
	int point=0;
	int i=0;
	if(str[0]=='-')
		i=1;
	for(;i<size;i++)
	{
		r=str[i];
		if(r=='.')
		{
			if(point==1 || i==0)
			{	
				err=1;
				i=size;
			}
			else
				point=1;
		}
		else if(r<'0' || r>'9') 
		{
			err=1;
			i=size;
		}
	}
	if(err==0)
		*a=atof(str);
	return err;
}

expr creation_aux(char** str,int* pos,int* err)
{
	int i=*pos;
	*pos+=1;
	if(str[i]==NULL)
		return NULL;
	else
	{
		expr e=(expr)malloc(sizeof(struct abin));
		e->fd=NULL;
		e->fg=NULL;
		int isbin=0;
		double a;
		int c=strtocle(str[i]);
		switch(c)
		{
			case B1:/* + */
			case B2:/* - */
			case B3:/* * */
			case B4:/* / */
			case B5:/* min */
			case B6:/* max */
				e->descri=c;
				isbin=1;
				break;
			case U1:/* sqrt */
			case U2:/* exp */
			case U3:/* ln */
			case U4:/* abs */
			case U5:/* floor */
			case U6:/* ceil */
				e->descri=c;
				break;				
			case A1:
				if(return_nb(str[i],&a)==0)
				{
					e->descri=c;
					e->val=a;
					break;
				}
				else
				{
					write(2,"Operateur ou operande inconnu :",31);
					write(2,str[i],strlen(str[i]));
					write(2,"\n",2);
					printf("ERROR\n");
					*err=1;
					return e;
				}
		}
		if(e->descri!=13)
		{
			if(isbin==0)
			{
				if(str[*pos]==NULL)
				{
					write(2,str[i],strlen(str[i]));
					write(2," attend un argument\n",21);
					printf("ERROR\n");
					*err=1;
					return e;
				}
				e->fd=NULL;
				e->fg=creation_aux(str,pos,err);
			}
			else
			{
				if(str[*pos]==NULL)
				{
					write(2,str[i],strlen(str[i]));
					write(2," attend deux arguments\n",24);
					printf("ERROR\n");
					*err=1;
					return e;
				}
				e->fd=creation_aux(str,pos,err);
				if(str[*pos]==NULL)
				{
					write(2,str[i],strlen(str[i]));
					write(2," attend deux arguments\n",24);
					printf("ERROR\n");
					*err=1;
					return e;
				}
				e->fg=creation_aux(str,pos,err);
			}
		}
		return e;
	}
}

expr creation(char* str)
{
	int err=0;
	int i,j=2;
	for(i=0;str[i]!='\0';i++)
	{
		if(str[i]==' ')
			j++;
	}
	char** s=(char**)malloc(j*sizeof(char*));
	char* token=strtok(str," ");
	i=0;
	while(token!=NULL)
	{
		s[i]=(char*)malloc(strlen(token)+1);
		strcpy(s[i],token);
		token=strtok(NULL," ");
		i++;
	}
	s[i]=NULL;
	i=0;
	expr res=creation_aux(s,&i,&err);
	if(err==1)
	{
		free_expr(res);
		free_mem(s);
		return NULL;
	}
	free_mem(s);
	return res;
}

double calcul_expr(expr e, int* err)
{
	if(e==NULL)
	{
		*err=1;
		return 0.0;
	}
	double a,b;
	int c;
	switch(e->descri)
	{
		case B1:/* + */
			a=calcul_expr(e->fd,err)+calcul_expr(e->fg,err);
			break;
		case B2:/* - */	
			a=calcul_expr(e->fd,err)-calcul_expr(e->fg,err);
			break;
		case B3:/* * */
			a=calcul_expr(e->fd,err)*calcul_expr(e->fg,err);
			break;
		case B4:/* / */
			b=calcul_expr(e->fg,err);
			if(b==0.0)
			{
				write(2,"Division par 0: impossible\n", 28);
				printf("ERROR\n");
				*err=1;
			}
			a=calcul_expr(e->fd,err)/b;
			break;
		case B5:/* min */
			a=calcul_expr(e->fd,err);
			b=calcul_expr(e->fg,err);
			if(a>b)
				a=b;
			break;
		case B6:/* max */
			a=calcul_expr(e->fd,err);
			b=calcul_expr(e->fg,err);
			if(a<b)
				a=b;
			break;
		case U1:/* sqrt */
			b=calcul_expr(e->fg,err);
			if(b<0.0)
			{
				write(2,"Racine carre d'un negatif: impossible\n", 39);
				printf("ERROR\n");
				*err=1;
			}
			a=sqrt(b);
			break;
		case U2:/* exp */
			a=exp(calcul_expr(e->fg,err));
			break;
		case U3:/* ln */
			b=calcul_expr(e->fg,err);
			if(b<=0.0)
			{
				write(2,"ln d'un negatif: impossible\n", 29);
				printf("ERROR\n");
				*err=1;
			}
			a=log(b);
			break;
		case U4:/* abs */
			a=calcul_expr(e->fg,err);
			if(a<0.0)
				a*=(-1.0);
			break;
		case U5:/* floor */
			a=floor(calcul_expr(e->fg,err));
			c = 0 + (int)a;
			a=(double)c;
			break;
		case U6:/* ceil */	
			a=ceil(calcul_expr(e->fg,err));
			break;
		case A1:
			a=e->val;
			break;
	}
	return a;
}
