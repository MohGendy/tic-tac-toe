#include "ai.h"
#include <iostream>

State * State::free(){//post order traversal to free memory
    for(int i = 0;i<9;i++){
        if(this->nextState[i] != NULL){
            this->nextState[i] = this->nextState[i]->free();
        }
    }
    delete this;
    return NULL;
}


Tree::Tree(bool start){
    this->head=minimax(start,0);
    this->current=this->head;
}

Tree::~Tree(){
    head = head->free();
}

bool Tree::move(int move,bool isPlayer){
    if(current->nextState[move]!=NULL){
        this->board[move]=(isPlayer?2:1);
        current = current->nextState[move];
        return true;
    }else{
        return false;
    }
}

bool Tree::bestMove(int * move){
    int bestRate = -101;
    int best = -1;
    State * bestState = NULL;
    for(int i =0;i<9;i++){
        if(current->nextState[i]!= NULL){
            // std::cout << i << " : " << current->nextState[i]->rate << std::endl;
            if(bestRate < current->nextState[i]->rate){
                bestState = current->nextState[i];
                best = i;
                bestRate = bestState->rate;
            }
        }
    }
    if(best == -1){
        return false;
    }else{
        *move = best;
        return true;
    }

}

State * Tree::minimax(bool isMaximizing, int depth, int lastMove) {
    State * temp = new State;

    // If last move led to a win, return a terminal state
    if (lastMove > -1 && this->checkBoard(lastMove) == 1) {
        temp->rate = (isMaximizing ? -100/depth : 100/depth);
        return temp;
    }

    // If board is full, it's a draw
    if (this->isFull()) {
        temp->rate = 0;
        return temp;
    }

    // Initialize rate conservatively
    temp->rate = (isMaximizing ? -101 : 101);

    // Generate child states
    for (int i = 0; i < 9; i++) {
        if (this->board[i] == 0) { // If cell is empty
            this->board[i] = (isMaximizing ? 1 : 2); // Assign move
            temp->nextState[i] = minimax(!isMaximizing, depth + 1, i);
            temp->rate = (isMaximizing ? std::max(temp->nextState[i]->rate, temp->rate)
                                       : std::min(temp->nextState[i]->rate, temp->rate));
            this->board[i] = 0; // Undo move
        }
    }

    return temp;
}

int Tree::checkBoard(int move){
    int symbol = board[move];
    int x = move/3 , y = move%3;
    if((this->board[x*3+0]==symbol && this->board[x*3+1]==symbol && this->board[x*3+2]==symbol)||
    (this->board[y]==symbol && this->board[y+3]==symbol && this->board[y+6]==symbol)||
    (this->board[0]==symbol&&this->board[4]==symbol&&this->board[8]==symbol)||
    (this->board[2]==symbol&&this->board[4]==symbol&&this->board[6]==symbol)){
        return 1;
    }else{
        return 0;
    }
}

bool Tree::isFull(){
        for(int i = 0;i<9;i++){
            if(this->board[i]==0){
                return false;
            }
        }
        return true;
}

//only to debug ,not used now
void Tree::print(){
    for(int i = 0;i<9;i++){
        std::cout << " " << board[i];
        if(i%3==2) std::cout << std::endl;
    }
}

bool Tree::rates(int arr[]){
    for(int i = 0 ; i < 9 ; i++){
        if(current->nextState[i]!= NULL){
            std::cout << i << " : " << current->nextState[i]->rate << std::endl;
            arr[i] = current->nextState[i]->rate;
        }
    }
    return true;
}

void Tree::treeBoard(int arr[]){
    for(int i = 0; i<9;i++){
        arr[i] = board[i];
    }
}

void Tree::reset(){
    current = head;
    for (int i = 0; i < 9; i++)
    {
        board[i] = 0;
    }
}


Ai::Ai(int start,int level):tree(Tree(start)){
    this->level = level;
}

bool Ai::moveAi(int * move){
    bool flag = false;
    int board[9] = {0};
    int seed;
    switch (level){
        case easy:
            seed = rand()%9;
            tree.treeBoard(board);
            while(1){
                if(board[seed] == 0){
                    *move = seed;
                    tree.move(seed,false);
                    return true;
                }else{
                    seed = (seed+1)%9;
                }
            };
        break;
        case normal:
            seed = rand()%100;
            if(seed < 50){
                flag = tree.bestMove(move);//
                tree.move(*move,false);
                return flag;
            }else{
                int rates[9];
                tree.treeBoard(board);
                tree.rates(rates);
                int max = -101 , index = -1;
                for(int i = 0; i < 9 ; i++){
                    if(board[i]==0&&rates[i]>max){
                        max = rates[i];
                        index = i;               
                    }
                }
                seed %= 9;
                while(1){
                    if(board[seed] == 0 && seed != index){
                        *move = seed;
                        tree.move(seed,false);
                        return true;
                    }else{
                        seed = (seed+1)%9;
                    }
                };
            }

        break;
        case hard: // change later
                flag = tree.bestMove(move);//
                tree.move(*move,false);
                return flag;
        break;
        
    }
    *move = -1; // if no move is possible
    return false;
    
}
bool Ai::movePlayer(int move){
    return tree.move(move,1);
}

void Ai::print(){
    tree.print();
}

void Ai::reset(){
    tree.reset();
}