#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;



class Password{
	private:
		string password;
		string passwordType;
	public:
		string getPasswordType(){
		return passwordType;
		}
		string getPassword(){
		return password;
		}
		void setPasswordType(string tempPasswordType){
			passwordType=tempPasswordType;
		}
		void setPassword(string tempPassword){
			password=tempPassword;
		}
	
};

int serialSearch(Password passwords[10000],string passwordType,int passwordCount){
	int i=0;
	while(i<passwordCount){
		if(passwords[i].getPasswordType()==passwordType){
		return i;
	}
		i++;
	}
	return -1;
	}
	
class FileManager{
	private:
		string fileName="passwords.txt";
		
	public:
		
	string openFile(){
		string line;
		
		ifstream file ("passwords.txt");
		
		if(file.is_open()){
			cout<<"file Open\n";
			
			stringstream buffer; //it's a string that u can add new things,but it doesn't read like string
			buffer << file.rdbuf();//read the whole text and returns it like buffer
			file.close();
			return buffer.str();
		
		}else{
			cout<<"creating password file";
			ofstream file;
			file.open("passwords.txt");
			file.close();
		}
		
			
		}
		
		string getFileName(){
			return fileName;
		}
		
		void saveToFile(Password passwords[10000],int passwordCount){
			
			ofstream file("passwords.txt");
			string line;
			
			if(file.is_open()){
				for(int i=0;i<passwordCount;i++){
					line=passwords[i].getPasswordType();
					line=line + ":";
					line=line + passwords[i].getPassword()+"\n";
					file<<line;
					
				}
				file.close();
			}else{
				cout<<"Unable to open file\n";
			}
			
		}
			
	
};
class PasswordManager{
	private:
		Password passwords[10000];
		FileManager fileManager;
		int passwordCount;
		
	public:
		PasswordManager (){
			passwordCount=-1;
		}
		//ftiaxtei ta passwords/ apo string parse tis grammes se type Password
		void parsePasswords(){
			if(passwordCount!=-1){
				return;//to count einai-1 otan den tous exei trabijei pote tous kodikous/gia na min ta janatrabaei
			}
			string line;
			string passwordType;
			string password;
			
			string fileText=fileManager.openFile();
			stringstream stream(fileText);
			passwordCount=0;
			while(getline(stream,line)){//with this getline we get the whole line
				int pos=line.find(":");
				passwordType=line.substr(0,pos);
				password=line.substr(pos+1);
				
				
				passwords[passwordCount].setPasswordType(passwordType);
				passwords[passwordCount].setPassword(password);
				passwordCount++;
				
			
			}			
			
			
		}
		void createNewPassword(){
			string passwordType;
			string password;
			
			parsePasswords(); //gia na fortothoun oi kodikoi
			
			cout<<"Password Type: ";
			cin>>passwordType;
			passwords[passwordCount].setPasswordType(passwordType);
			cout<<"Password: ";
			cin>>password;
			passwords[passwordCount].setPassword(password);
			
			passwordCount++;
			
			
			fileManager.saveToFile(passwords,passwordCount);
			cout<<"Password created sucessfully\n";
			
				}
				
		void viewPasswords(){
			parsePasswords();
			cout<<passwordCount<<" Total passwords:\n";
			for(int i=0 ;i<passwordCount;i++){
				cout<<"password number "<<i<<endl;
				cout<<passwords[i].getPasswordType()<<endl;
				cout<<passwords[i].getPassword()<<endl;
				cout<<"------------------------------------"<<endl;
			}	
		}
		
		void generatePassword(){
			parsePasswords();
			
			string passwordType;
			string password="";
			int length;
			string charset;
			int charSize;
			int randomNum;
			
			cout<<"Password Type: ";
			cin>>passwordType;
			passwords[passwordCount].setPasswordType(passwordType);
			
			cout<<"Enter the password length\n";
			cin>>length;
			
			charset="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+=<>?";
			srand(static_cast<unsigned int>(time(0)));
			for(int i=0;i<length;i++){
				charSize=charset.size();
				randomNum= rand()%charSize;
				password=password+charset[randomNum];
			}
			passwords[passwordCount].setPassword(password);
			passwordCount++;
		
			fileManager.saveToFile(passwords,passwordCount);
			cout<<"Password Generated sucessfully,new password:"<<password<<"\n";
		}	
		
		void deletePassword(){
			parsePasswords();
			string passwordType;
			int pos,i;
			
			if(passwordCount==0){
				cout<<"there are no passwords\n";
				return; 
			}
			viewPasswords();
			cout<<"Enter the password you want to delete:\n";
			cin>>passwordType;
			
			pos=serialSearch(passwords,passwordType,passwordCount);
			
			if(pos==-1){
				cout<<"Invalid input\n";
				return;
			}
			for(i=pos;i<passwordCount;i++){
				passwords[i]=passwords[i+1];
			}
			passwordCount--;
			viewPasswords();
			
			fileManager.saveToFile(passwords,passwordCount);
			cout<<"Password deleted succesfully\n";
			
			
			
		}
		
		void editPassword(){
			parsePasswords();
			string passwordType;
			int pos,choice;
			string password;
			string charset="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+=<>?";
			
			if(passwordCount==0){
				cout<<"there are no passwords\n";
				return; 
			}
			viewPasswords();
			
			cout<<"Enter the password you want to edit:\n";
			cin>>passwordType;
			
			pos= serialSearch(passwords,passwordType,passwordCount);
			
			if(pos==-1){
				cout<<"Invalid input\n";
				return;
			}
			
			cout<<"1.Custom input\n";
			cout<<"2.Generate\n";
			cout<<"3.Back\n";
			cin>>choice;
			switch(choice){
				case 1:
					cout<<"Password: ";
					cin>>password;
					passwords[pos].setPassword(password);
					fileManager.saveToFile(passwords,passwordCount);
					break;
				case 2:
					password="";
					int length;
					
					int charSize;
					int randomNum;
					cout<<"Enter the password length\n";
					cin>>length;
			
					
					srand(static_cast<unsigned int>(time(0)));
					for(int i=0;i<length;i++){
						charSize=charset.size();
						randomNum= rand()%charSize;
						password=password+charset[randomNum];
						}
					passwords[pos].setPassword(password);
					fileManager.saveToFile(passwords,passwordCount);	
					break;
				case 3:
					return;
					break;	
					
			}
		}
	};
	
class Menu{
	public:
		void show(){
			cout<<"PASSWORD MANAGER\n";
			cout<<"---------------------\n";
			cout<<"1.View Passwords\n";
			cout<<"2.Add new Password\n";
			cout<<"3.Generate new Password\n";
			cout<<"4.Edit Password\n";
			cout<<"5.Delete Password\n";
			cout<<"6.Exit\n";
		}
			
	};

/*class Authentication{
	function encrypt password with ceasar and the key will be the password
	function for decrypt
	function for login(in the begining tha sigrine ton kodiko me hash, and idf the password was correct tha decrypt all the password -using the key as the password-)
};*/
int main() {

	int choice;
	PasswordManager passwordManager;
	Menu menu;
	
	
	
	do{
		menu.show();
		cin>>choice;
		switch(choice){
			case 1:
				passwordManager.viewPasswords();
				break;
			case 2:
				passwordManager.createNewPassword();
				break;
			case 3:
				passwordManager.generatePassword();
				break;
			case 4:
				passwordManager.editPassword();
				break;
			case 5:
				passwordManager.deletePassword();
				break;				
		}
	}while(choice !=6);

	
	
	return 0;
}
