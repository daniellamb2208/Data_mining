#include<iostream>
#include<vector>
#include<fstream>
#define quantity 41
using namespace std;

//first : 76.7132%

class bayes{
    public:
    	bayes(){
    		True = 0;
    		False = 0;
		}
        void start();
        void train_read();
        void training();
        void verify();
        int predict(unsigned long long int pos);
        void output_read();
        void output();
    private:
        vector<double*> data;
        vector<int> answer;
        double probability_table[quantity][4];
        unsigned long long int True,False;
};
void bayes::start(){
	cout<<"start\n";
    train_read();
    training();
    verify();
    output_read();
    output();
}
void bayes::train_read(){
	cout<<"reading\n";
    ifstream file("NSL-KDDTrain+.txt");
    while(!file.eof()){
        double* tmp = new double [quantity];
        int ans; 
        for (int i = 0; i < quantity; i++){
            file >> tmp[i];
            file.ignore();
        }
        file>>ans;
        data.push_back(tmp);
        answer.push_back(ans);
    }
    file.close();
}

void bayes::training(){
	cout<<"training\n";
    unsigned long long int training_size = answer.size()*4/5;

    for(int i=0;i<training_size;i++){
        if (answer[i])
            True++;
        else
            False++;
    }

    for(int i=0;i<quantity;i++){
        double ff(0),ft(0),tf(0),tt(0);
        unsigned long long int T = True, F = False;
        for(unsigned long long int j=0;j<training_size;j++){
            if (!answer[j] && !data[j][i])
                ff++;
            else if (!answer[j] && data[j][i])
                ft++;
            else if (answer[j] && !data[j][i])
                tf++;
            else
                tt++;
        }
        if (!tt||!tf){
        	tt++;
        	tf++;
        	T++;
		}
		else if (!ft||!ff){
			ft++;
			ff++;
			F++;
		}
        probability_table[i][0] = ff/F;
        probability_table[i][1] = ft/F;
        probability_table[i][2] = tf/T;
        probability_table[i][3] = tt/T;
    }
}
void bayes::verify(){
	cout<<"verify\n";
    unsigned long long int start_pos = answer.size()*4/5+1;
	double yes(0),no(0);
    for(unsigned int i=start_pos;i<answer.size();i++){
    	int ans = answer[i];
    	if (ans == predict(i))
    		yes++;
    	else
    		no++;
    }
    cout<<"accuracy : "<<yes/(answer.size()-start_pos)<<endl;
    cout<<"wrong : "<<no/(answer.size()-start_pos)<<endl;
    /*for(unsigned long long int i=0;i<data.size();i++){
    	data[i].clear();
	}*/
	data.clear();
}

int bayes::predict(unsigned long long  int pos){
    double prob1(1),prob2(1);
    for(int i=0;i<quantity;i++){
        if (data[pos][i]){
        	prob1 *= probability_table[i][3];
        	prob2 *= probability_table[i][1];
		}
		else{
			prob1 *= probability_table[i][2];
        	prob2 *= probability_table[i][0];
		}
    }
    return prob1 > prob2 ? 1 : 0; 
}
void bayes::output_read(){
	cout<<"reading\n";
    ifstream file("NSL-KDDTest+.txt");
    while(!file.eof()){
        double* tmp = new double [quantity]; 
        for (int i = 0; i < quantity; i++){
            file >> tmp[i];
            if (i != quantity-1)
            	file.ignore();
        }
        data.push_back(tmp);
    }
    file.close();
    
}
void bayes::output(){
	ofstream out("a.txt",ios::out);
	for(unsigned long long int i=0;i<data.size();i++){
		out<<predict(i);
	}
	cout<<data.size()<<endl;
	out.close();
	cout<<"done\n";
}
int main(){
    bayes a;
    a.start();
    return 0;
}
