//MATHEUS PERUSSO E VINICIUS LIRA
#include <iostream>
#include <string>
#include <vector>
#include <iomanip> 
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;
int cache_Type, block_size, tamanhoCache, block_number=0;

int compulsry_misses=0, capcity_misses=0, conflict_misses=0;


#define		DRAM_SIZE		(64*1024*1024)

unsigned int m_w = 0xABABAB55;  
unsigned int m_z = 0x05080902;


unsigned int rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}
bool cacheSim(unsigned int addr, int cash[3][100000], int type, int &block_counter, int index_addr, int tag_addr)
{
    int shift_offset=log2(block_size);
    bool detected=false;
    bool misses_flag=true;
    
    if (cache_Type==0)   
    {
        if (cash[0][index_addr]==tag_addr)
        {
            return true;
        }
        else
            {
                cash[0][index_addr]= tag_addr;
                for (int i=0; i < block_number; i++)
                {
                    if (cash[1][i]!=1)
                    {   misses_flag=false;
                        i=block_number;}
                    
                }
                //misses
                if (misses_flag)
                    capcity_misses++;  
                else
                {
                    if(cash[1][index_addr]==1)
                        conflict_misses++;
                    else
                    {
                        compulsry_misses++;
                    }
                }
                cash[1][index_addr]= 1;
                return 0;
            }
    } 
    
    else if (cache_Type==1)  
    {
        index_addr=index_addr * type;
        for (int i=0; i < type ; i++)
        {
            if (cash[0][index_addr+i]==tag_addr)
            {
               
                return 1;
            }
        }
        for (int j=0; j < type; j++)
        {
            if (cash[1][index_addr+j] == -1)
            {
                compulsry_misses++;
                cash[0][index_addr+j]=tag_addr;
                cash[1][index_addr+j]=1;
                return 0;
            }
        }
        
        srand(time(NULL));
        int x=rand()%(type);
        cash[0][index_addr+x]=tag_addr;
        cash[1][index_addr+x]=1;
        capcity_misses++;
        return 0;
        
    }
    
    
    else if (cache_Type==2)       
        if (type==0)    
            if (block_counter < block_number)
            {
                for (int i=0; i < block_number; i++)
                {
                    if (cash[0][i]==addr >> shift_offset)
                    {
                        detected=true;
                        cash[1][i]=block_counter;
                        block_counter--;
                        return detected; //hit
                    }
                }
                
                if (!detected)
                {
                    compulsry_misses++;
                    cash[0][block_counter]=addr>>shift_offset;
                    cash[1][block_counter]=block_counter;
                    return false;  //miss
                }
            }
            else  
            {
             
                for (int i=0; i < block_number; i++)
                {
                    if (cash[0][i]==(addr >> shift_offset))
                    {
                        detected=true;
                        cash[1][i]=block_counter;
                        //block_counter--;
                        return detected; //hit
                    }
                }
                
                if (!detected)
                {
                    int compare=0;
                    for (int i=1; i < block_number; i++)
                    {
                        if (cash[1][compare] > cash[1][i])
                            compare=i;
                    }
                    cash[0][compare]=addr >> shift_offset;
                    cash[1][compare]=block_counter;
                    capcity_misses++;
                    return false; //miss

                }
            }
           
        
        else if (type==1)   
        {
            if (block_counter < block_number)
            {
                for (int i=0; i < block_number; i++)
                {
                    if (cash[0][i]==addr >> shift_offset)
                    {
                        detected=true;
                        cash[1][i]=cash[1][i]+1;
                        block_counter--;
                        return detected; //hit
                    }
                }
                
                if (!detected)
                {
                    cash[0][block_counter]=addr>>shift_offset;
                    cash[1][block_counter]=-1;
                    compulsry_misses++;
                    return false;  //miss
                }
            }
            else 
            {
                for (int i=0; i < block_number; i++)
                {
                    if (cash[0][i]==(addr >> shift_offset))
                    {
                        detected=true;
                        cash[1][i]++;
                        block_counter--;
                        return detected; 
                    }
                }
                if (!detected)
                {
                    int compare2=0;
                    for (int i=1; i < block_number; i++)
                    {
                        if (cash[1][compare2] >= cash[1][i])
                            compare2=i;
                    }
                    cash[0][compare2]=addr >> shift_offset;
                    cash[1][compare2]=-1;
                    capcity_misses++;
                    return false;
                }
            }

        } 
        
        else if (type==2)
        {
            if (block_counter < block_number)
            {
                for (int i=0; i < block_number; i++)
                {
                    if (cash[0][i]==addr >> shift_offset)
                    {
                        detected=true;
                        block_counter--;
                        return detected; 
                    }
                }
                
                if (!detected)
                {
                    cash[0][block_counter]=addr>>shift_offset;
                    cash[1][block_counter]=block_counter;
                    compulsry_misses++;
                    return false; 
                }
            }
            else  
                
                for (int i=0; i < block_number; i++)
                {
                    if (cash[0][i]==(addr >> shift_offset))
                    {
                        detected=true;
                        //block_counter--;
                        return detected; //hit
                    }
                }
                
                if (!detected)
                {
                    int compare=0;
                    for (int i=1; i < block_number; i++)
                    {
                        if (cash[1][compare] > cash[1][i])
                            compare=i;
                    }
                    cash[0][compare]=addr >> shift_offset;
                    cash[1][compare]=block_counter;
                    capcity_misses++;
                    return false; 
                }
            }
        
        else if (type==3)
        {
            if (block_counter < block_number)
            {
                for (int i=0; i < block_number; i++)
                {
                    if (cash[0][i]==addr >> shift_offset)
                    {
                        detected=true;
                        block_counter--;
                        return detected; 
                    }
                }
                
                if (!detected)
                {
                    cash[0][block_counter]=addr>>shift_offset;
                    compulsry_misses++;
                    return false;  
            }
            else  
            {
                for (int i=0; i < block_number; i++)
                {
                    if (cash[0][i]==(addr >> shift_offset))
                    {
                        detected=true;
                        
                        return detected;
                    }
                }
                
                if (!detected)
                {
                    srand(time(NULL));
                    cash[0][rand()%block_number]=addr >> shift_offset;
                    capcity_misses++;
                    return 0; 
                }
            }
        }
    }   
    return true;
}


unsigned int gen1()
{
    static unsigned int addr=0;
    return (addr++)%(DRAM_SIZE);
}

unsigned int gen2()
{
    static unsigned int addr=0;
    return  rand_()%(128*1024);
}

unsigned int gen3()
{
    return rand_()%(DRAM_SIZE);
}

unsigned int gen4()
{
    static unsigned int addr=0;
    return (addr++)%(1024);
}

unsigned int gen5()
{
    static unsigned int addr=0;
    return (addr++)%(1024*64);
}

unsigned int gen6()
{
    static unsigned int addr=0;
    return (addr+=256)%(DRAM_SIZE);
}



char *msg[2] = {"Miss","Hit"};

int main(int argc, const char * argv[]) {
    
    int looper=1000000, addr, flag, shift;

    cout << "(0) Mapeamento direto - (1)Set associativo - (2) Totalmente associativo: " << endl;
    cin >> cache_Type;
    cout << "Selecione o tamanho do bloco (valor entre 4 e 128 bytes): " << endl;
    cin >> block_size;
    cout << "Selecione o tamanho da cache( 1KB - 64KB):: " << endl;
    cin >> tamanhoCache;
    
    

    int cash[3][100000];
    int block_counter=0;
    int hit_counter=0;
    int index_addr=0, tag_addr=0;
    
    if ( cache_Type==0) 
    {
        block_number= (tamanhoCache*1024)/block_size;
        
        for (int i=0; i < 2; i++)   
            for (int j=0; j < block_number; j++)
                cash[i][j]=-1;
        
        for(int i=0; i <looper ;i++)
        {
            addr = gen1();
            shift= log2(block_size);
            index_addr= (addr >> shift)% block_number;
            shift= log2(block_number+block_size);
            tag_addr= addr >>shift;    
            flag = cacheSim(addr, cash, 0,block_counter, index_addr, tag_addr);
            index_addr=0;
            tag_addr=0;
            cout <<"0x" << setfill('0') << setw(8) << hex << addr <<" ("<< msg[flag] <<")\n";
            if (msg[flag]=="Hit")
            {
                hit_counter++;
            }
        }
    cout << "Hits  " << hit_counter<<endl << "Compulsry:  " << compulsry_misses <<endl<< "Capcity:  " << capcity_misses <<endl<< "Conflict:  " << conflict_misses <<  endl;

    }
    else if (cache_Type==2) 
    {
        int replacment_type;
        block_number= (tamanhoCache*1024)/block_size;
        
        cout << "Tipo de substituição de endereço para o totalmente associativo: LRU->0 , LFU->1, FIFO->2, RANDOM->3  :- " << endl;
        cin >> replacment_type;
        
        
        for (int i=0; i < 2; i++) 
            for (int j=0; j < block_number; j++)
                cash[i][j]=-10;
        
        for(int i=0; i <looper ;i++)
        {
            addr = gen4();
            flag = cacheSim(addr, cash, replacment_type, block_counter, index_addr, tag_addr);
           
            if (msg[flag]=="Hit")
            {
                hit_counter++;
            }
            block_counter++;

        }
        
        cout << "Hits  " << hit_counter<<endl << "Compulsry:  " << compulsry_misses <<endl<< "Capcity:  " << capcity_misses <<endl<< "Conflict:  " << conflict_misses <<  endl;
        
    } 
    else if (cache_Type==1) 
    {
        int number_of_ways;
        cout << "Insira o número de maneiras para o set associativo de cache(2,4,8,16): " << endl;
        cin >> number_of_ways;
        block_number= (tamanhoCache*1024)/(block_size*number_of_ways);
        
        for (int i=0; i < 3; i++)  
            for (int j=0; j < 100000; j++)
                cash[i][j]=-1;
        
        for(int i=0; i <looper ;i++)
        {
            addr = gen5();
            shift= log2(block_size);
            index_addr= (addr >> shift)% (block_number);
            shift= log2(block_number+block_size);
            tag_addr= addr >>shift;
            flag = cacheSim(addr, cash, number_of_ways, block_counter, index_addr, tag_addr);            
            index_addr=0;
            tag_addr=0;
            if (msg[flag]=="Hit")
            {
                hit_counter++;
            }
            block_counter++;
        }
        
        cout << "Hits  " << hit_counter<<endl << "Compulsry:  " << compulsry_misses <<endl<< "Capcity:  " << capcity_misses <<endl<< "Conflict:  " << conflict_misses <<  endl;
    }
  
}
