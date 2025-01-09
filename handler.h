//
// Created by abratosin on 1/9/25.
//

#ifndef HANDLER_H
#define HANDLER_H


// clasă abstractă care îmi definește interacțiunile cu CSV-urile

// aici doar am pus toată definiția în header
class Handler {
protected:
    string file_path;
public:
    virtual ~Handler() = default;

    explicit Handler(const string& file_path_in){
        file_path = file_path_in;
    }

    virtual void parse_data() = 0;
    virtual vector<vector<string>> get_data() = 0;
    virtual void write_to_file() = 0;
};



#endif //HANDLER_H