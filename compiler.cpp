#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<vector>
#include<stack>
#include<stdlib.h>

#define K_DIGIT       3      //����
#define K_CHAR        4      //�ַ�
#define K_STRING      5      //�ַ���
#define K_TYPE        6      //��������
#define K_KEYWORDS    7      //�ؼ���
#define K_OPERATOR    8      //�����
#define K_IDENTIFIER  9      //��ʶ��
#define K_BRACKET     10     //����

using namespace std;

//�洢�ִ�����
typedef struct IDwords
{
	int       id;     //��־
	string    word;   //����
}IDwords;

typedef struct Variable
{
	string   var;    //����
	string   value;  //��ʼ��ֵ
}Variable;

//Ŀ�����Ԫ��
typedef struct Target
{
	string    dsf;    //���
	string    op;     //����
	string    dst;    //Ŀ�Ĳ�����
	string    dsc;    //Դ������
	string    mark;   //��־
	string    step;   //��תλ��
}Target;

//������������
vector<Variable>      var_table;
//����Ŀ�����
vector<Target>        target_code;


char                  lab='A'; //��¼��ת��־
char                  vab='A'; //��¼�м����

//���ɵĻ���ļ�����
string  asmfile(string source)
{
	if(source.size()==0)
	{
		cout<<"Դ�ļ�������Ϊ��"<<endl;
		exit(-1);
	}
	string temp="";
	int i,j;
	j = source.size();
    for(i = j-1;i>=0;i--)
	{
//		if(source[i] == '\\' || source[i]== '/')
//			break;
		if(source[i] == '.')
		{
			j = i;
			break;
		}
	}
	temp = source.substr(0,j) + ".asm";
	return temp;
}

//��������ȼ�
int level(string s)
{
	if(s=="#")
		return 1;
	else if(s=="=")
		return 2;
	else if(s=="+" || s=="-")
		return 3;
	else if(s=="*" || s=="/")
		return 4;
	else
		return -1;
}


//���浽Ŀ�����
void add_target_code(string dsf,string op,string dst,string dsc,string mark,string step)
{
	Target  tmp;
	tmp.dsf = dsf;
	tmp.op = op;
	tmp.dst = dst;
	tmp.dsc = dsc;
	tmp.mark = mark;
	tmp.step = step;
	target_code.push_back(tmp);
}

//�ַ�ת�ַ���
string char_to_str(char c)
{
	char s[2] = " ";
	s[0] = c;
	return string(s);
}

//�Ƿ�Ϊ���������
int is_operator(char c)
{
	if(c == '+' || c=='-'||c=='*'||c=='/'||c==','||c=='=' ||c=='>' || c=='<')
		return 1;
	else
		return 0;
}

//�Ƿ�Ϊ�����š�С���š��ֺ�
int is_bracket(char c)
{
	if(c=='{' || c=='}' || c=='(' || c==')' ||c==';')
		return 1;
	else
		return 0;
}

//�Ƿ�Ϊ�հ�
int is_blank(char c)
{
	if(c=='\n' || c=='\t' || c==' ' || c=='\r')
		return 1;
	else
		return 0;
}

//�жϵ�������
int word_token(string s)
{
	int    size = s.size();
	//�ַ�����
	if(s[0]=='\'')
	{
		if(s[size-1] == '\'')
			return K_CHAR;
		else
		{
			cout<<"������ַ������ݣ�"<<s<<endl;
			exit(-1);
		}
	}
	//�ַ�������
	else if(s[0]=='\"')
	{
		if(s[size-1]=='\"')
			return K_STRING;
		else
		{
            cout<<"������ַ������ݣ�"<<s<<endl;
			exit(-1);
		}
	}
	//����
	else if(isdigit(s[0]))
	{
		for(int i=1;i<size;i++)
		{
			if(!isdigit(s[i]))
			{
				cout<<"���Ϸ��ı�ʶ����"<<s<<endl;
				exit(-1);
			}
		}
		return K_DIGIT;
	}
	else
	{
		for(int i=0;i<size;i++)
		{
			if(!isalnum(s[i]) && s[i]!='_')
			{
                cout<<"���Ϸ��ı�ʶ����"<<s<<endl;
				exit(-1);
			}
		}
		//��������
		if(s=="int" || s=="char")
			return K_TYPE;
		//�ؼ���
		else if(s=="if" || s=="else" || s=="printf" || s=="main")
			return K_KEYWORDS;
		//�Զ����ʶ��
		else
			return K_IDENTIFIER;
	}
}

//��ӷִʽ��
void add_keywords(vector<IDwords> &v,int id,string word)
{
	IDwords    temp;
	temp.id = id;
	temp.word = word;
	v.push_back(temp);
}

//�ʷ�����
void lexical_analysis(string source,vector<IDwords> &AnalysisResults)
{
	char       ch;
	ifstream   rfile(source.c_str());
	if(!rfile.is_open())
	{
		cout<<"�޷���Դ�ļ�"<<endl;
		exit(-1);
	}

	rfile>>noskipws;   //�����˿ո�
	while(rfile>>ch)
	{
		int         state=0;        //�ж�״̬
		string      temp("");       //�ַ�������
        char        try_ch;         //̽��ǰ����ַ�
		
		switch(state)
		{
		case 0:
			if(ch=='/') //������ע��
			{
				rfile>>try_ch;
				if(try_ch=='/')
				{
					while(rfile>>try_ch)
					{
						if(try_ch=='\n')
							break;   //����һ��ע��
					}
					break;
				}
				else if(try_ch=='*')
				{
					while(rfile>>try_ch)
					{
						if(try_ch=='*')
						{
							rfile>>try_ch;
							if(try_ch=='/')
								break; //���Ƕ���ע��
						}
					}
					break;
				}
				else
				{
					add_keywords(AnalysisResults,K_OPERATOR,char_to_str(ch));
					ch = try_ch;  //����״̬1
				}
			}
		case 1:
		    if(is_operator(ch)) //�жϲ�����
			{
				add_keywords(AnalysisResults,K_OPERATOR,char_to_str(ch));
				break;
			}
		case 2:
		    if(is_bracket(ch)) //�����š�С����
			{
				add_keywords(AnalysisResults,K_BRACKET,char_to_str(ch));
				break;
			}
		case 3:
			if(is_blank(ch)) //�հ׷�
				break;
		case 4:
			if(ch=='#') //����Ԥ����
			{
				while(rfile>>ch)
				{
					if(is_blank(ch))
					{
						break;
					}
				}
				break;
			}
		default://�жϵ�������
			temp = temp + char_to_str(ch);
			while(rfile>>try_ch)
			{
				if(try_ch == '\"')
				{
					temp = temp + char_to_str(try_ch);
					if(ch == '\"')
					{
						add_keywords(AnalysisResults,word_token(temp),temp);
						break;
					}
					else
					{
						cout<<"���Ϸ��ı�ʶ����"+temp<<endl;
						exit(-1);
					}
				}
				else if(is_blank(try_ch) )
				{
					if(ch != '\'' && ch != '\"')
					{
						add_keywords(AnalysisResults,word_token(temp),temp);
						break;
					}
					else
						temp = temp + char_to_str(try_ch);
				}
				else if(is_operator(try_ch) )
				{
					if(ch !='\'' && ch != '\"' )
					{
						add_keywords(AnalysisResults,word_token(temp),temp);
					    add_keywords(AnalysisResults,K_OPERATOR,char_to_str(try_ch));
						break;
					}
					else
						temp = temp + char_to_str(try_ch);
				}
				else if(is_bracket(try_ch))
				{
					add_keywords(AnalysisResults,word_token(temp),temp);
					add_keywords(AnalysisResults,K_BRACKET,char_to_str(try_ch));
					break;
				}
				else
					temp = temp + char_to_str(try_ch);
			}
		}
	}
	rfile.close();
}

//����ʷ��������
void print_lexical(vector<IDwords> &v)
{
	vector<IDwords>::iterator  it;
	for(it = v.begin();it != v.end();it++)
		cout<<it->id<<" "<<it->word<<endl;
}

//��ȡ��������
void add_var_table(vector<IDwords>::iterator &it)
{
	while(it->id == K_TYPE)
	{
		it++;
		while(it->word != ";")
		{
	
			if(it->id == K_IDENTIFIER)
			{
				Variable     tmp;
				tmp.var = it->word;
				string   tmp_var = it->word;
				if((it+1)->word=="=")   //�жϱ�����û�г�ʼ��
				{
					it = it+2;
					tmp.value = it->word;
					add_target_code(tmp_var,"=",tmp.value," "," "," ");
				}
				var_table.push_back(tmp);
			}
			it++;
		}
		it++;
	}
}

//���ʽ����
void expression(vector<IDwords>::iterator &it)
{
	string dsf,op,dst,dsc;         
	//����ǲ�����ջ
    stack<string>         word_stack;
    //������ջ
    stack<string>         oper_stack;
	oper_stack.push("#");
	while(it->word != ";")       //����';'һ��������
	{
	
		if(it->word == "(")
			oper_stack.push(it->word);
		else if(it->word == ")")
		{

			while(oper_stack.top() != "(")
			{
				op = oper_stack.top();
			
			    oper_stack.pop();
		//	    oper_stack.push(it->word);
			    dsc = word_stack.top();
                word_stack.pop();
			    dst = word_stack.top();
			    word_stack.pop();
			    vab = vab+1;
				if(vab == 91)
					vab = '0';
			    dsf = "tmp" + char_to_str(vab);

				Variable     tmp;
				tmp.var = dsf;
				var_table.push_back(tmp);

			    word_stack.push(dsf);
                add_target_code(dsf,op,dst,dsc," "," ");
			}
			oper_stack.pop();
	
		}
		else if(it->id != K_OPERATOR)
			word_stack.push(it->word);
		else if(oper_stack.top() == "(")
		{
			oper_stack.push(it->word);
		}
		else if(level(it->word) < level(oper_stack.top()))  //���ȼ���
		{
			op = oper_stack.top();
			oper_stack.pop();
			oper_stack.push(it->word);
			dsc = word_stack.top();
            word_stack.pop();
			dst = word_stack.top();
			word_stack.pop();
			vab = vab+1;
			if(vab == 91)
				vab = '0';
			dsf = "tmp" + char_to_str(vab);

            Variable     tmp;
			tmp.var = dsf;
			var_table.push_back(tmp);

			word_stack.push(dsf);
            add_target_code(dsf,op,dst,dsc," "," ");
		}
		else   //���ȼ���
			oper_stack.push(it->word);
		it++;
	}
	//����ʣ�µ�
	while(oper_stack.top() != "#")
	{
		op = oper_stack.top();
        oper_stack.pop();
		dsc = word_stack.top();
        word_stack.pop();
		dst = word_stack.top();
		word_stack.pop();
	
		if(op=="=")//��ֵ����
		{
			add_target_code(dst,op,dsc," "," "," ");
		}
		else
		{
            vab = vab+1;
			if(vab == 91)
				vab = '0';
			dsf = "tmp" + char_to_str(vab);

            Variable     tmp;
			tmp.var = dsf;
			var_table.push_back(tmp);

			word_stack.push(dsf);
            add_target_code(dsf,op,dst,dsc," "," ");
		}
	}
}

//����printf���
void printf_analysis(vector<IDwords>::iterator &it)
{
	int j,i=1;
	it = it+2;
	string str = it->word; //��ȡ�������
	string strvar;         //��ȡ�������

	Variable       tmp;
	//����������ݼ���ʽ
	for(j=1;j<str.size()-1;)
	{
		if(str[j]=='%')
        {
			if(i != j)
			{
				vab = vab + 1;
				if(vab == 91)
					vab = '0';
			    add_target_code("\'"+str.substr(i,j-i)+"$\'","p"," "," ","tmp"+char_to_str(vab)," ");
			    tmp.var = "tmp"+char_to_str(vab);
			    tmp.value = "\'"+str.substr(i,j-i)+"$\'";
			    var_table.push_back(tmp);
            }

			i = j+2;
			it = it+2;  //��ȡ��Ӧ����
			strvar = it->word;
            add_target_code(strvar,"p"," "," ",str.substr(j,2)," ");
			j = i;
			continue;
		}
		j++;
	}
	if(i!=j)
	{
		vab = vab+1;
		if(vab == 91)
			vab = '0';
		add_target_code("\'"+str.substr(i,j-i)+"$\'","p"," "," ","tmp"+char_to_str(vab)," ");
		tmp.var = "tmp"+char_to_str(vab);
		tmp.value = "\'"+str.substr(i,j-i)+"$\'";
		var_table.push_back(tmp);
	}
	it = it+2; //�Թ���)}��
}

//����if���
void if_analysis(vector<IDwords>::iterator &it)
{
	string  op,mark,dst,dsc;
	it++;
	if(it->word != "(")
	{
		cout<<"�����if��䣺ȱ��'('"<<endl;
		exit(-1);
	}
	it++;
	dst = it->word;
	it++;
	mark = it->word;
	it++;
	dsc = it->word;
	op = "if";

	add_target_code(" ",op,dst,dsc,mark,"step"+char_to_str(lab+1));
	it++;
	if(it->word != ")")
	{
		cout<<"�����if������䣺ȱ��')'"<<endl;
		exit(-1);
	}
	it++; //�Թ���{��
	it++;
    
	//����else
	vector<IDwords>::iterator   it2 = it;
    while(it2->word != "}")
	{
		it2++;
	}
	it2++;
	//�ж���û��else
	if(it2->word == "else")
	{
		it2++; //�Թ���{��
        while(it2->word != "}")
		{
			expression(it2);
			it2++;
		}
	}//else�������
	else
		it2--;
	lab = lab + 2;
    add_target_code(" ","jmp"," "," "," ","step"+char_to_str(lab));

	add_target_code(" ","pstep"," "," "," ","step"+char_to_str(lab-1));
	
	while(it->word != "}")
	{
		expression(it); //���ʽ����
		it++;
	}
    add_target_code(" ","jmp"," "," "," ","step"+char_to_str(lab));
    add_target_code(" ","pstep"," "," "," ","step"+char_to_str(lab));

	it = it2;
}

//�﷨����
void syntax_analysis(vector<IDwords> &AnalysisResults)
{
	vector<IDwords>::iterator  it=AnalysisResults.begin();
    if(it->word != "main")
	{
		cout<<"ȱ��main"<<endl;
		exit(-1);
	}
	it = it+3; //������������
	if(it->word != "{")
	{
		cout<<"main����ȱ��'{'"<<endl;
		exit(-1);
	}
	it++;
	//��ȡ��������
    add_var_table(it);
    //��ȡ����εĲ���
	while(it != AnalysisResults.end())
	{
		//����printf
		if(it->word == "printf")
		{
			printf_analysis(it);
		}
		// if ���
		else if(it->word == "if")
		{
			if_analysis(it);
		}
		else if(it->word == "}")
			break;
		//���ʽ����
		else
		{
			expression(it); //���ʽ����
		}
		it++;
	}
}

//����﷨�������
void print_syntax()
{
	vector<Variable>::iterator  it;
	cout<<"������������ʼ��"<<endl;
	for(it = var_table.begin();it != var_table.end();it++)
	{
		cout<<it->var<<"   "<<it->value<<endl;
	}

	vector<Target>::iterator it2;
	cout<<"�м����"<<endl;
	for(it2 = target_code.begin();it2 != target_code.end();it2++)
	{
		cout<<it2->dsf<<"  "<<it2->op<<"  "<<it2->dst<<"  "<<it2->dsc<<"  "<<it2->mark<<"  "<<it2->step<<endl;
	}
}

//�Ӽ���ת��
void addsub_asm(ofstream &out,string dsf,string op,string dst,string dsc)
{
	out<<"    mov BL,"<<dst<<endl;
	if(op == "+")
		out<<"    add BL,"<<dsc<<endl;
	else
		out<<"    sub BL,"<<dsc<<endl;
	out<<"    mov "<<dsf<<",BL"<<endl;
}

//�˷�
void mul_asm(ofstream &out,string dsf,string dst,string dsc)
{
	out<<"    mov AL,"<<dst<<endl;
	out<<"    mov BH,"<<dsc<<endl;
	out<<"    mul BH"<<endl;
	out<<"    mov BL,1"<<endl;
	out<<"    div BL"<<endl;
	out<<"    mov "<<dsf<<",AL"<<endl;
}

//����
void div_asm(ofstream &out,string dsf,string dst,string dsc)
{
	out<<"    mov AL,"<<dst<<endl;
	out<<"    CBW"<<endl;
	out<<"    mov BL,"<<dsc<<endl;
	out<<"    div BL"<<endl;
	out<<"    mov "<<dsf<<",AL"<<endl;
}

//��ֵ����
void sign_asm(ofstream &out,string dsf,string dst)
{
	out<<"    mov BL,"<<dst<<endl;
	out<<"    mov "<<dsf<<",BL"<<endl;
}

//���ת��
void print_asm(ofstream &out,string dsf,string mark)
{
	//���ַ���ʽ���
	if(mark=="%c")
	{
		out<<"    mov DL,"<<dsf<<endl;
		out<<"    mov AH,02H"<<endl;
		out<<"    int 21H"<<endl;
	}
	//��������ʽ���
	else if(mark=="%d")
	{
		out<<"    mov AL,"<<dsf<<endl;
		out<<"    CBW"<<endl;
		out<<"    mov BL,10"<<endl;
		out<<"    DIV BL"<<endl;
		out<<"    mov BH,AH"<<endl;
		out<<"    add BH,30H"<<endl;
		out<<"    add AL,30H"<<endl;
		out<<"    CMP AL,48"<<endl;
		//ȷ��ʮλ�Ƿ���0
		lab = lab + 2;
		string step2 = "step" + char_to_str(lab);
		out<<"    JE "<<step2<<endl;
        string step1 = "step" + char_to_str(lab-1);
		out<<"  "<<step1<<":"<<endl;
		out<<"    mov DL,AL"<<endl;
		out<<"    mov AH,2"<<endl;
		out<<"    int 21H"<<endl;

		//�����λ
		out<<"  "<<step2<<":"<<endl;
		out<<"    mov DL,BH"<<endl;
		out<<"    mov AH,2"<<endl;
		out<<"    int 21H"<<endl;
	}
	//�ַ������
	else
	{
		out<<"    LEA DX,"<<mark<<endl;
		out<<"    mov AH,09"<<endl;
		out<<"    int 21H"<<endl;
	}
}

//if���ת��
void if_asm(ofstream &out,string dst,string dsc,string mark,string step)
{
	out<<"    mov AL,"<<dst<<endl;
	out<<"    CMP AL,"<<dsc<<endl;
	if(mark == ">")
		out<<"    JG "<<step<<endl;
	else if(mark == "<")
		out<<"    JL "<<step<<endl;
	else
	{
		cout<<"�ݲ�֧�����������ж�"<<endl;
		exit(-1);
	}
}


//���ɻ���ļ�
void create_asm(string file)
{
	//��������
	ofstream   wfile(file.c_str());
	if(!wfile.is_open())
		cout<<"�޷���������ļ�"<<endl;

	vector<Variable>::iterator  it_var;
	
	wfile<<"ASSUME CS:codesg,DS:datasg"<<endl;
	//���ݶ�
	wfile<<"datasg segment"<<endl;
	for(it_var=var_table.begin();it_var!=var_table.end();it_var++)
	{
		wfile<<"    "<<it_var->var<<" DB ";
		if(it_var->value != "")
			wfile<<it_var->value<<endl;
		else
			wfile<<"\'?\'"<<endl;
	}
	wfile<<"datasg ends"<<endl;
	//�����
	wfile<<"codesg segment"<<endl;
	wfile<<"  start:"<<endl;
	wfile<<"    mov AX,datasg"<<endl;
	wfile<<"    mov DS,AX"<<endl;

	vector<Target>::iterator     it;
	Target        tmp;
    for(it = target_code.begin();it != target_code.end();it++)
	{
		//�Ӽ���ת��
		if(it->op == "+" || it->op=="-")
			addsub_asm(wfile,it->dsf,it->op,it->dst,it->dsc);
		//�˷�ת��
		else if(it->op == "*")
			mul_asm(wfile,it->dsf,it->dst,it->dsc);
		//����ת��
		else if(it->op == "/")
			div_asm(wfile,it->dsf,it->dst,it->dsc);
		//��ֵ����
		else if(it->op == "=")
			sign_asm(wfile,it->dsf,it->dst);
		//�������
		else if(it->op == "p")
			print_asm(wfile,it->dsf,it->mark);
		//if�﷨����
		else if(it->op == "if")
		{
			if_asm(wfile,it->dst,it->dsc,it->mark,it->step);
		}
		else if(it->op == "else")
		{
			cout<<"else û���ҵ�ƥ��� if"<<endl;
			exit(-1);
		}
		//��ת���
		else if(it->op == "jmp")
		{
			wfile<<"    JMP "<<it->step<<endl;
		}
		//��ת���α�ʶ
		else if(it->op == "pstep")
		{
			wfile<<"  "<<it->step<<":"<<endl;
		}
		//����
		else
		{
			cout<<"�������ݲ�֧�ָ��﷨����"<<endl;
			exit(-1);
		}
	}

	//����ν���
	wfile<<"    mov ax,4C00H"<<endl;
	wfile<<"    int 21H"<<endl;
	wfile<<"codesg ends"<<endl;
	wfile<<"  end start"<<endl;

	wfile.close();
}


int main(int argc,char* argv[])
{
	vector<IDwords>  AnalysisResults;

	string source;

	//ȱʡʱ������ʾ
	if(argc == 1)
	{
		cout<<"*****************************************************"<<endl;
		cout<<"\n��Դ�ļ�Ŀ¼������ .asm ����ļ�\n"<<endl;
		cout<<"*****************************************************"<<endl;
		cout<<"\n������Դ�ļ���";
		cin>>source;
        //�ʷ�����
	    lexical_analysis(source,AnalysisResults);
		//�﷨����
	    syntax_analysis(AnalysisResults);
		//���ɻ���ļ�
	    create_asm(asmfile(source));
		cout<<"\nʹ�ý���"<<endl;
	}
	else if(argc == 2)
	{
		//Ĭ�����ɻ���ļ���Դ�ļ���.asm
        //�ʷ�����
	    lexical_analysis(argv[1],AnalysisResults);
		//�﷨����
	    syntax_analysis(AnalysisResults);
		//���ɻ���ļ�
	    create_asm(asmfile(argv[1]));
	}
	else if(argc == 3)
	{
		//�ʷ�����
	    lexical_analysis(argv[1],AnalysisResults);
		//�﷨����
	    syntax_analysis(AnalysisResults);
		//���ɻ���ļ�
	    create_asm(asmfile(argv[2]));
	}
	else
	{
		cout<<"��ȷʹ�ø�ʽ��compile.exe [Դ�ļ�] [����ļ�]"<<endl;
	}

	//�������
 //   print_lexical(AnalysisResults);
	//����﷨�������
//	print_syntax();
	return 0;
}




