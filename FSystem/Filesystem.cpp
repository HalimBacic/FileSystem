//
// Created by halim on 6.9.19..
//

#include "Filesystem.h"


using namespace std;

Filesystem::Filesystem() {
    superblok = Superblok();
    memory = Memory();
    bitmap=Bitmap();
    for(auto & i : inode)
        i=Inode();
    this->info=this->createRoot();
}

bool Filesystem::createRoot() {
    //Postavlja root inode
     this->inode[0].setFlag(1);
     this->inode[0].setName("root");
     this->inode[0].setPath("root");
     this->inode[0].setSize(0);
     this->inode[0].setType(false);
    this->bitmap.setInode(0,1);   //Zauzimanje u bitmapi
     //

    //Kreira System.bin
     this->file.open("System.bin",ios::binary |  ios::app);
     if(!file) {
         file.write((char *) &this->inode[0], sizeof(inode));   //Upisuje root inode u System.bin
         for (int i = 1; i < NUM; i++)     //Upis Inodova
             file.write((char *) &this->inode[i], sizeof(this->inode));
         for(int i=0;i<NUM;i++)           //Upisuje memorijske blokove
         file.write("0",64);

         return true;
     }
     file.close();
     return false;
}

bool Filesystem::createFoDir(std::string path, std::string name,bool t) {
    int i=this->bitmap.getInode();    //Pronalaženje inodea u bitmapi
    if(i==75) return false;           //Ako su svi inodovi zauzeti, nemoguće je smještati u memoriju
    if(!this->memory.getCapacity()) return false;   //Ako je memorija popunjena
    this->bitmap.setInode(i,1);       //Zauzimanje inoda u bitmapi
    this->inode[i].setFlag(1);        //Postavljanje zauzetosti inodea
    this->inode[i].setName(name);     //Postavljanje imena
    this->inode[i].setPath(path+"/"+name);   //Postavljanje putanje
    this->inode[i].setSize(0);        //Postavljanje veličine fajla. 0 je jer je tek kreirano
    this->inode[i].setType(t);        // ako je t=true tada je fajl, a ako je t=false tada je folder
    this->inode[i].setAddr(0,0);
    int temp=sizeof(inode);
    this->memory.reduce(-temp);
    this->file.open("System.bin",ios::binary |  ios::out | ios::app);   //Otvaranje datoteke za promjenu linije ovog inodea
    if(file)
    {
        file.seekg(i*sizeof(inode));    //Postavljanje read pokazivača na mjesto
        file.write((char*)&this->inode[i],sizeof(Inode));  //Upis inodea
        file.close();
        return true;
    } else
    {  cout<<"Error.\n";  return false; }
}

void Filesystem::listingPath(std::string upath) {
    for(int i=1;i<NUM;i++)
        if(this->inode[i].comparePath(upath) && this->inode[i].getName()!="Unknown")
            std::cout<<this->inode[i].getName()<<"  ";
}

void Filesystem::stat(std::string path) {
    std::cout<<"  NUM: "<<" NAME: "<<" PATH: "<<" ADDRESS: "<<" FILE_SIZE: \n";
    int i;
    for(i=0;i<NUM && path!=this->inode[i].getPath();++i);
    std::cout<<i<<": "<<this->inode[i].getName()<<": "<<this->inode[i].getPath()<<": ";
 //   this->inode[i].getAddrs();
    std::cout<<": "<<this->inode[i].getSize()<<"\n";
}

bool Filesystem::echo(std::string path, std::string text) {
    file.open("System.bin", ios::app | ios::binary);
    if(file && this->memory.getCapacity()) {
        int i;
        for (i = 0; i < NUM && this->inode[i].getPath() != path; ++i);   //pronalazi se inode sa path-om
        this->inode[i].setSize(text.length()+this->inode[i].getSize());      //Postavljanje novog size-a za dati fajl i inode
        this->inode[i].setAddr(file.tellp(),text.length());      //Postavljaju se adrese za početak i kraj zapisa
        file.write(text.data(), text.length());     //Upis teksta, upisuje text.lenght() bitova
        file.close();
        this->memory.reduce(-text.length());          //Oduzimanje prostora u memoriji
        return true;
    }
    return false;
}

void Filesystem::deleteSystem() {
     ::remove("System.bin");
}

void Filesystem::renameName(std::string path, std::string new_name) {
    int i = path.length();                                    //Promjena imena datoteke
    while (path[i] != '/') i--;
    std::string path1 = path;
    path1.erase(i + 1, path.length());
    std::string path2 = path1 + new_name;
    for (i = 0; i < NUM && this->inode[i].getPath() != path; ++i);
    this->inode[i].setName(new_name);
    this->inode[i].setPath(path2);
    this->file.open("System.bin",ios::out | ios::binary);
    file.seekp(0);
    file.seekp(i*sizeof(Inode)+1);
    file.write((char*)&this->inode[i],sizeof(Inode));
    file.close();
}

void Filesystem::move(std::string path1, std::string path2) {      //path1 = gdje pomjeram path2 = šta pomjeram
    int i;
    for(i=0;i<NUM && this->inode[i].getPath()!=path2;++i);
    this->inode[i].setPath(path1+"/"+this->inode[i].getName());  //Promjena putanje jer se fajl premješta na drugu putanju
    this->file.open("System.bin",ios::binary | ios::out);     //Ne premješta se i u memoriji
    file.seekp(i*sizeof(Inode));
    file.write((char*)&this->inode[i],sizeof(Inode));
    file.close();
}

void Filesystem::copy(std::string copyWhat, std::string copyWhere) {    //Kopira datoteku copyWhat na putanju copyWhere
    int i; //i za copyWhat
    for(i=0;i<NUM && this->inode[i].getPath()!=copyWhat;++i); //Pronalazak inodea sa copyWhat
    if(this->createFoDir(copyWhere,this->inode[i].getName(),this->inode[i].getType()))   //Izrada novog inodea u copyWhere
    {//Kopiranje sadržaja iz blokova copyWhat u copyWhere
        for (int k = 0; this->inode[i].getPair(k).first != 0; k++)   //Uzima parove linija na osnovu kojih se traži sadržaj fajla
        {
            pair<int, int> temp = this->inode[i].getPair(k);    //start i end
            char input[temp.second - temp.first];
            string text = "";
            this->file.open("System.bin", ios::binary | ios::in);
            file.seekg(temp.first);    //Postavljanje pokazivača za čitanje
            file.read((char *) &input,temp.second - temp.first);   //Čitanje input sadržaja koji se upisuje na drugi blok  -1
            file.close();
            input[temp.second-temp.first]='\0';
            text = input;
            cout << text;
            this->echo(copyWhere + "/" + this->inode[i].getName(), text.data());    //Upis linije u os
        }
    } else
        cout<<"Error.";
}

void Filesystem::remove(std::string path, std::string opt) {

      if(opt.compare("-r")==0)
      {
          for(int i=1;i<NUM && this->inode[i].comparePath(path);i++)
              remove(this->inode[i].getPath(),"any");
      }

      int i;
      for(i=0;i<NUM && this->inode[i].getPath()!=path;++i);  //pronalazak inodea za brisanje

      this->memory.reduce(-this->inode[i].getSize());
      int temp=sizeof(inode);
      this->memory.reduce(temp);
      //Brisanje u memoriji
    for(int k=0;this->inode[i].getPair(k).first!=0;k++)     //Prolazim kroz adrese inode-a
    {
        pair<int, int> temp = this->inode[i].getPair(k);    //start i end
        this->file.open("System.bin",ios::binary | ios::out);
        file.write("0",temp.second-temp.first);
        file.close();
    }
      this->bitmap.setInode(i,0);
      this->inode[i].resetInode();
}

void Filesystem::cat(std::string path) {
    int i;
    for(i=0;i<NUM && this->inode[i].getPath()!=path;++i);   //Pronalazim inode na putanji

    for(int k=0;this->inode[i].getPair(k).first!=0;k++)     //Prolazim kroz adrese inode-a
    {
        pair<int, int> temp = this->inode[i].getPair(k);    //start i end
        char input[temp.second-temp.first];
        this->file.open("System.bin", ios::binary | ios::in | ios::app);
        file.seekg(temp.first);    //Postavljanje pokazivača za čitanje
        file.read(input,temp.second - temp.first);   //Čitanje input sadržaja koji se upisuje na drugi blok
        string text;
        input[temp.second-temp.first]= '\0';
        text=input;
        cout<<text.data()<<"\n";
        file.close();
    }
}

void Filesystem::upload(std::string path) {   //path do fajla koji uploadujemo
    int i;
    for(i=0;i<NUM && this->inode[i].getPath()!=path;++i);   //Pronalazi inode
    fstream data;
    data.open(this->inode[i].getName(),ios::app | ios::out);   //Kreiranje datoteke
    for(int k=0;this->inode[i].getPair(k).first!=0;k++)     //Prolazim kroz adrese inode-a
    {
        pair<int, int> temp = this->inode[i].getPair(k);    //start i end
        char input[temp.second-temp.first];
        this->file.open("System.bin",ios::binary | ios::in);
        file.seekg(temp.first);
        file.read((char*)&input,temp.second-temp.first);
        input[temp.second-temp.first]='\0';
        string datatext;
        datatext=input;
        cout<<datatext;
        data<<datatext;
        file.close();
    }
    data.close();
}

void Filesystem::download(std::string pathToFile, std::string path) {
    string name=pathToFile;    //Koristi se za kreiranje novog fajla u sistemu
    for(int i=name.length();i>0;i--)
        if(name[i]=='/')
        {
            name.erase(0,i+1);    //Ime fajla na putanji
            i=0;
        }
    if(this->createFoDir(path,name,true))   //Napravljen inode za novi fajl na sistemu
    {
        fstream data;
        string x;
        data.open(pathToFile); //Otvara fajl za čitanje, binarno
        string outtext = "";
        while (getline(data,x))   //dok ne dođe do kraja inode-a
        {
            outtext +=x;
        }
        this->echo(path+"/"+name, outtext);
        data.close();
    } else
        cout<<"Error.";
}

void Filesystem::free() {
    this->superblok.print();
    std::cout<< "Free Memory: " <<   this->memory.getCapacity() <<"kB\n";
    std::cout << "  NUM: " << " NAME: " << " PATH: " << " ADDRESS: " << " FILE_SIZE: \n";
    int i;
    for (i = 0;i<75; i++) {
        if (this->inode[i].getFlag()) {
            cout << i << ": " << this->inode[i].getName() << ": " << this->inode[i].getPath() << ": ";
            this->inode[i].getAddMem();
            cout << ": " << this->inode[i].getSize() << "\n";
        }
    }
}

void Filesystem::find(std::string fname) {
    std::cout<<"Results:\n";
    for(int i=0;i<75;i++)
        if(fname==this->inode[i].getName())
            std::cout<<this->inode[i].getPath();
}

void Filesystem::help() {
    fstream data;
    string line;
    data.open("help.txt");
    while(getline(data,line))
       cout<<line<<"\n";
}
