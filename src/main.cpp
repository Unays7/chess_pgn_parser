#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>

using namespace std;

struct info{
    string event_name;
    string site;
    string date;
    int round;
    string white;
    string black;
    string result;
    void print(){
        cout << "Event : "<< event_name << endl;
        cout << "Site  : "<< site << endl;
        cout << "Date  : "<< date << endl;
        cout << "Round : "<< round<< endl;
        cout << "White : "<< white<< endl;
        cout << "Black : "<< black<< endl;
        cout << "Result: "<< result<<endl;
        cout << endl;
    }
}game;
string get_important_info(){
    string s;
    getline(cin, s);
    string ret = "";
    int t = 0;
    for(auto c : s){
        if(c == '\"'){
            t ^= 1;
            continue;
        }
        if(t == 1){
            ret += c;
        }
    }
    return ret;
}
void getEvent(){
    game.event_name = get_important_info();
}
void getSite(){
    game.site = get_important_info();
}
void getDate(){
    game.date = get_important_info();
}
void getRound(){
    string round = get_important_info();
    reverse(round.begin(), round.end());
    game.round = 0;
    for(auto c : round){
        game.round *= 10;
        game.round += (int)(c - '0');
    }
}
void getWhite(){
    game.white = get_important_info();
}
void getBlack(){
    game.black = get_important_info();
}
void getResult(){
    game.result = get_important_info();
}
void LoadBasicInfo(){
    getEvent();
    getSite();
    getDate();
    getRound();
    getWhite();
    getBlack();
    getResult();
}

vector<pair<int, pair<string, string> > > moves;

void ReadTheGame(){
    string s;
    string S = "";
    while(getline(cin, s)){
        bool comment = false;
        string aux = "";
        for(int i = 0; i < (int)s.size(); i ++){
            if(s[i] == ';') break;
            if(s[i] == '{'){
                comment = true;
                continue;
            }
            if(s[i] == '}'){
                comment = false;
                continue;
            }
            if(comment){
                continue;
            }
            aux += s[i];
        }
        s = aux;
        aux = "";
        for(int i = 0; i < (int)s.size(); i ++){
            aux += s[i];
            if(s[i] == ' '){
                while(i + 1 < (int)s.size() && s[i + 1] == ' '){
                    i ++;
                }
            }
        }
        s = aux;
        while(!s.empty() && s.back() == ' ') s.pop_back();
        s += ' ';
        S += s;
    }
    s = S;
    for(int i = 0; i < (int)s.size(); i ++){
        if(s[i] == '.'){
            //this part is not necessary since
            //we already keep track of the number
            //of moves done till this position.
            int j = i - 1;
            int move  = 0;
            int pow10 = 1;
            while(j >= 0 && '0' <= s[j] && s[j] <= '9'){
                move = move + pow10 * ((int)(s[j] - '0'));
                pow10 *= 10;
                j --;
            }
            moves.push_back({move, {"", ""}});
            
            if(s[i+1] == ' '){
                j = i + 2;
            }
            else{
                j = i + 1;
            }
            int empty_space = 0;
            while(j < (int)s.size() && empty_space < 2){
                if(s[j] == ' '){
                    empty_space ++;
                    j ++;
                    continue;
                }
                if(empty_space == 0){
                    moves.back().second.first  += s[j];
                }
                else{
                    moves.back().second.second += s[j];
                }
                j ++;
            }
        }
    }
}

int T[8][8];
int F[8][8];

char f[6] = {'P', 'N', 'R', 'B', 'Q', 'K'};

void print_board(){
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j ++){
            if(T[i][j] == -1){
                cout << "  ";
            }
            else{
                if(T[i][j] == 0){
                    cout << 'w';
                }
                else{
                    cout << 'b';
                }
                cout << f[F[i][j]];
            }
            if(j != 7){
                cout << '|';
            }
        }
        cout << endl;
    }
    cout << endl;
    
}

vector<pair<int,int> > pos[2][6];

int Nr[8] = {+2, +2, -2, -2, +1, +1, -1, -1};
int Nc[8] = {+1, -1, +1, -1, +2, -2, +2, -2};
 
int Br[4] = {+1, +1, -1, -1};
int Bc[4] = {+1, -1, +1, -1};

int Rr[4] = {+1, 0,-1, 0};
int Rc[4] = { 0,+1, 0,-1};

bool inside(int r, int c){
    return (0 <= r && r < 8 && 0 <= c && c < 8);
}

void take_out(int r, int c){
    int p = T[r][c];
    int fig = F[r][c];
    
    T[r][c] = -1;
    F[r][c] = -1;
    for(int i = 0; i < (int)pos[p][fig].size(); i ++){
        if(pos[p][fig][i].first == r && pos[p][fig][i].second == c){
            swap(pos[p][fig][i], pos[p][fig].back());
            pos[p][fig].pop_back();
            break;
        }
    }
}
void put_in(int r, int c, int p, int fig){
    T[r][c] = p;
    F[r][c] = fig;
    pos[p][fig].push_back({r, c});
}

void castle(int p, int t){
    int row = 0;
    if(p == 0){
        row = 7;
    }
    int crook;
    int cking = 4;
    if(t == 0){
        crook = 7;
    }
    else{
        crook = 0;
    }
    take_out(row, crook);
    take_out(row, cking);

    if(t == 0){
        crook = 5;
        cking = 6;
    }
    else{
        crook = 3;
        cking = 2;
    }

    put_in(row, cking, p, 5);
    put_in(row, crook, p, 2);
}

bool reach(int f, int r, int c, int dr, int dc){
    if(f == 1){
        for(int i = 0; i < 8; i ++){
            int NR = r + Nr[i];
            int NC = c + Nc[i];
            if(NR == dr && NC == dc){
                return true;
            }
        }
        return false;
    }
    if(f == 2){
        for(int i = 0; i < 4; i ++){
            int NR = r + Rr[i];
            int NC = c + Rc[i];
            while(inside(NR, NC)){
                if(NR == dr && NC == dc) return true;
                
                //obstacle
                if(F[NR][NC] != -1) break;
                
                NR += Rr[i];
                NC += Rc[i];
            }
        }
        return false;
    }
    if(f == 3){
        for(int i = 0; i < 4; i ++){
            int NR = r + Br[i];
            int NC = c + Bc[i];
            while(inside(NR, NC)){
                if(NR == dr && NC == dc) return true;
                
                //obstacle
                if(F[NR][NC] != -1) break;
                
                NR += Br[i];
                NC += Bc[i];
            }
        }
        return false;
    }
    if(f == 4){
        for(int i = 0; i < 4; i ++){
            int NR = r + Br[i];
            int NC = c + Bc[i];
            while(inside(NR, NC)){
                if(NR == dr && NC == dc) return true;
                
                //obstacle
                if(F[NR][NC] != -1) break;
                
                NR += Br[i];
                NC += Bc[i];
            }
        }
        for(int i = 0; i < 4; i ++){
            int NR = r + Rr[i];
            int NC = c + Rc[i];
            while(inside(NR, NC)){
                if(NR == dr && NC == dc) return true;
                
                //obstacle
                if(F[NR][NC] != -1) break;
                
                NR += Rr[i];
                NC += Rc[i];
            }
        }
        return false;
    }
    return false;
}

pair<int,int> take_pos(int p, string piece, int dr, int dc, bool cpt){
    char figure_type = piece[0];
    int ft = 0;
    while(f[ft] != figure_type) ft ++;
    
    if(ft == 5){
        return pos[p][ft][0];
    }
    
    int R = -1;
    int C = -1;
    if(piece.size() > 1){
        C = (piece[1] - 'a');
    }
    if(piece.size() > 2){
        R = 7 - (piece[2] - '0') + 1;
    }
    if(R != -1 && C != -1){
        return {R, C};
    }
    
    vector<pair<int,int> > cand;
    if(ft == 0){
        if(T[dr][dc] == -1){
            //pawn capturing en passant
            if(cpt){
                if(p == 0){
                    if(inside(dr + 1, dc + 1)){
                        cand.push_back({dr + 1, dc + 1});
                    }
                    if(inside(dr + 1, dc - 1)){
                        cand.push_back({dr + 1, dc - 1});
                    }
                }
                else{
                    if(inside(dr - 1, dc + 1)){
                        cand.push_back({dr - 1, dc + 1});
                    }
                    if(inside(dr - 1, dc - 1)){
                        cand.push_back({dr - 1, dc - 1});
                    }
                }
            }
            //pawn moving up/down
            else{
                if(p == 0){
                    if(T[dr + 1][dc] != -1){
                        cand.push_back({dr + 1, dc});
                    }
                    else{
                        cand.push_back({dr + 2, dc});
                    }
                }
                else{
                    if(T[dr - 1][dc] != -1){
                        cand.push_back({dr - 1, dc});
                    }
                    else{
                        cand.push_back({dr - 2, dc});
                    }
                }
            }
        }
        else{
            //pawn caputring another pawn - not en passant
            if(p == 0){
                if(inside(dr + 1, dc - 1)){
                    cand.push_back({dr + 1, dc - 1});
                }
                if(inside(dr + 1, dc + 1)){
                    cand.push_back({dr + 1, dc + 1});
                }
            }
            else{
                if(inside(dr - 1, dc - 1)){
                    cand.push_back({dr - 1, dc - 1});
                }
                if(inside(dr - 1, dc + 1)){
                    cand.push_back({dr - 1, dc + 1});
                }
            }
        }
    }
    else{
        for(auto POS : pos[p][ft]){
            if(reach(ft, POS.first, POS.second, dr, dc)){
                cand.push_back(POS);
            }
        }
    }
    if(C == -1){
        return cand[0];
    }
    for(auto POS : cand){
        if(POS.second == C){
            return POS;
        }
    }
    return {R, C};
}

pair<int,int> capture(int p, string move){
    int fig = 0;
    while(f[fig] != move[0]){
        fig ++;
    }
    string destination = "";
    while(move.back() != 'x'){
        destination += move.back();
        move.pop_back();
    }
    move.pop_back();
    int r = 7 - (int)(destination[0] - '0') + 1;
    int c = (int)(destination[1] - 'a');
    
    pair<int,int> pos = take_pos(p, move, r, c, true);

    take_out(pos.first, pos.second);
    take_out(r, c);
    
    put_in(r, c, p, fig);
    
    return {r, c};
}

pair<int,int> ordinary_move(int p, string move){
    int fig = 0;
    while(f[fig] != move[0]) fig ++;
    
    char c1 = move.back();
    move.pop_back();
    char c2 = move.back();
    move.pop_back();
    int dr = 7 - (int)(c1 - '0') + 1;
    int dc = (int)(c2 - 'a');
    
    pair<int,int> ps = take_pos(p, move, dr, dc, false);

    take_out(ps.first, ps.second);
    put_in(dr, dc, p, fig);
    return {dr, dc};
}

void make_move(int p, string move){
    //Since we only care what happens after the moves,
    //we don't care if a move is a check or a checkmate.
    if(move.back() == '#') move.pop_back();
    if(move.back() == '+') move.pop_back();
    
    if(move == "O-O"){
        castle(p, 0);
        return;
    }
    if(move == "O-O-O"){
        castle(p, 1);
        return;
    }
    
    if(!isupper(move[0])){
        move = "P" + move;
    }
    
    bool promotion = false;
    for(auto c : move){
        if(c == '='){
            promotion = true;
            break;
        }
    }
    if(promotion){
        char new_f = move.back();
        int fig = 0;
        while(f[fig] != new_f) fig ++;
        
        move.pop_back();
        move.pop_back();
        
        bool cpt = false;
        for(auto c : move){
            if(c == 'x'){
                cpt = true;
                break;
            }
        }
        if(cpt){
            pair<int,int> ps = capture(p, move);
            take_out(ps.first, ps.second);
            put_in(ps.first, ps.second, p, fig);
        }
        else{
            pair<int,int> ps = ordinary_move(p, move);
            take_out(ps.first, ps.second);
            put_in(ps.first, ps.second, p, fig);
        }
        return;
    }
    
    bool cpt = false;
    for(auto c : move){
        if(c == 'x'){
            cpt = true;
            break;
        }
    }
    if(cpt){
        capture(p, move);
    }
    else{
        ordinary_move(p, move);
    }
}
void set_board(){
    memset(F, -1, sizeof(F));
    memset(T, -1, sizeof(T));
    for(int i = 0; i < 8; i ++){
        T[0][i] = 1;
        T[1][i] = 1;
        F[1][i] = 0;
            
        T[6][i] = 0;
        T[7][i] = 0;
        F[6][i] = 0;
    }
    F[0][0] = F[0][7] = 2;
    F[0][1] = F[0][6] = 1;
    F[0][2] = F[0][5] = 3;
    F[0][3] = 4;
    F[0][4] = 5;
    
    F[7][0] = F[7][7] = 2;
    F[7][1] = F[7][6] = 1;
    F[7][2] = F[7][5] = 3;
    F[7][3] = 4;
    F[7][4] = 5;
    
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j ++){
            if(T[i][j] == -1) continue;
            pos[T[i][j]][F[i][j]].push_back({i, j});
        }
    }
}

void ProcessGame(){
    for(auto u : moves){
        string white_move = u.second.first;
        if(white_move == "1/2-1/2"){
            break;
        }
        bool halt = false;
        if(white_move.back() == '*'){
            halt = true;
        }
        string black_move = u.second.second;
        make_move(0, white_move);

        if(halt) break;
        
        if(black_move == "1/2-1/2"){
            break;
        }
        make_move(1, black_move);
    }
    print_board();
}
string file_path;
int main(int argc, char *argv[]){
    cin >> file_path;
    freopen(file_path.c_str(), "r", stdin);
    LoadBasicInfo();
    game.print();
    ReadTheGame();
    set_board();
    ProcessGame();
    return 0;
}
