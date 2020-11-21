
class WriteHTML{
private:
    std::mutex write_mutex; //write_mutex: used to lock the writing of html
    std::once_flag _flag;
    std::ofstream f;//f: where it is going to be written
public:
    WriteHTML(){}
    ~WriteHTML(){}
    void beginHTML(){
        std::call_once(_flag, [&](){f.open("output.html");});
        f << "<html>\n";
        f << "\t<head>\n";
        f << "\t\t<title>CORRECTION TEXT</title>\n";
        f << "\t</head>\n";
        f << "\t<body>\n";

        f << "<h1>CORRECTED TEXT</h1>\n";
    } //Write the beginning of the HTML
    void endHTML(){
        f << "\t</body>\n";
        f << "</html>\n";
    } //Write the end of the HTML
    void rightWord(const std::string& s){
        std::call_once(_flag, [&](){f.open("output.html");});
        std::unique_lock<std::mutex> locker(write_mutex, std::defer_lock);
        f << s << " ";
    } //Write if the word is correct
    void wrongWord(const std::string& s){
        std::call_once(_flag, [&](){f.open("output.html");});
        std::unique_lock<std::mutex> locker(write_mutex, std::defer_lock);
        f <<"<a style=\"color:red\">" << s << "</a> ";
    } //Write if the word is incorrect
    void checkWord(bool b, const std::string& s){//Checks the option it is going to be used
        if(b){
            rightWord(s);
        }else{
            wrongWord(s);
        }
    }
};

