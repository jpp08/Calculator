#include "expression.h"


int main(void)
{
    expr e;
    double result;
    int valid=0;
	char buf[1024];
	while((fgets(buf,1024,stdin))!= NULL)
	{
		buf[strlen(buf)-1]='\0';
		if(buf[0]!='\n' && buf[0]!='#')
		{
			e=creation(buf);
			if(e!=NULL)
			{
				result=calcul_expr(e,&valid);
				if(valid!=1)
					printf("%.4f\n",result);
			}
			free_expr(e);
		}
	}
	return 0;
}

