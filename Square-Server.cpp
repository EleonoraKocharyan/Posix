#include<signal.h>
#include <iostream>
#include  <string>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>
#include <sys/mman.h>
using namespace std;
int main()
{
    int*i;
    //creating a global variable for child and parent processes
    i=(int*)mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);

	*i=-1;//when -1 parent works, when 0 child works
    
	ofstream out;
    ifstream in;
	
	pid_t pid;
	pid=fork();

    if(pid!=0) //if parent
	{
		while(1)
		{   //number input
        	int x; 
            cout<<"enter n = ";
			cin>>x;
            //
			if(x!=0)
            {
				while(1)
                if(*i==-1)
                {   //writing input number in the file
                	out.open("iofile.txt" );
                    out<<x;
                    out.close();
                    //
                    *i=0; //giving the child permission to work
                    break;
                }

                while(1)
                if(*i==-1)
                {   //reading the square of input
                    int n;
                    in.open("iofile.txt");
                    in>>n;
                    in.close();
                    //
                    cout<<"n^2 = "<<n<<endl; //printing the result
                    break;
                }
            }
            else //input was 0
            {
                kill(pid,SIGKILL); //killing processes
                return 0;
            }      
		}
    }
        
	else
    {

        while(1)
        {
            while(1)
            if(*i==0)
            {   //reading the input number from the file 
				int n;
                in.open( "iofile.txt" );
                in>>n;
                in.close();
                //writing square of input number in the file
                n*=n;
            	out.open("iofile.txt");
                out<<n;
                out.close();
                //
                *i=-1; //giving the parent permission to work
                break;

            }
        }
    }
}


