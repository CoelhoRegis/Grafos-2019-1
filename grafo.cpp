//TP Grafos - Reginaldo  - 489012

// Aux. p/ DFS
#define WHITE 0
#define GRAY 1
#define BLACK 2

// Aux p/ PRIM
#define MAX 999999

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <list>
#include <stack>

using namespace std;

class Aresta{
public:
    int origem, destino;
    float peso;
    Aresta(int origem, int destino, float peso);
};

Aresta::Aresta(int origem, int destino, float peso){
    this->origem = origem;
    this->destino = destino;
    this->peso = peso;
}

class Vertice{    
public:
    Vertice(int nome);
    void print();
    int nome, cor, fim;
    float chave;
    vector<Aresta*> adj;
};

Vertice::Vertice(int nome){
    this->nome  = nome;
}

class Ponto{
public:
    int x, y;
    Ponto(int x, int y);
    float distancia(int a, int b);
};

Ponto::Ponto(int x, int y){
    this->x = x;
    this->y = y;
}

float Ponto::distancia(int x, int y){
    return sqrt( pow(this->x - x, 2) + pow(this->y - y, 2) ); 
}

class comparator{
public:
    int operator()(const Vertice* v1, const Vertice* v2){
        return(v1->chave > v2->chave);
    }
};

class Grafo{
    int num_vert, tempo, num_componente;
    vector<Vertice*> grafo;
    
public:
    bool bolada;
    Grafo(int num_vert);
    void add_aresta(int origem, int destino, float peso);
    void print();
    void dfs();
    void dfs_visita(Vertice* v);
    void prim(int v);
    void fortemente_conectado();
    Grafo transposto();
    void DFSUtil(int v, bool visitado[]);
    void fill_order(int v, bool visitado[], stack<int> &Stack);
    vector<int> dijkstra(int src);
    int caminhos(int num_cam, int num_lug);
    void all_paths(int src, int dst, bool imp, bool visited[], int path[], int &path_index, int &preco, int &num_cam, int num_lug, int &preco_final);
    bool is_safe(int v, int color[], int c);
    bool graph_colloring_util(int m, int color[], int v);
    bool graph_colloring(int m, int cores[]);
    void print_collor_solution(int color[]);
    void add_aresta_color(int num_aresta);
};

Grafo::Grafo(int num_vert){
    this->num_vert = num_vert;
    
    for(int i = 0; i < num_vert; i++ ){
        grafo.push_back(new Vertice(i));
    }

}

void Grafo::add_aresta(int origem, int destino , float peso){
    grafo[origem]->adj.push_back(new Aresta(origem, destino, peso));
    grafo[origem]->chave = peso;
}

void Grafo::print(){
    for (int i = 0; i < num_vert; i++){
        printf ("[%d]-> ", grafo[i]->nome);

        for (int j = 0; j < grafo[i]->adj.size(); j++){
            printf ("(%d, %.2f) -> ", grafo[i]->adj[j]->destino, grafo[i]->adj[j]->peso);
        }

        printf("\n");
    }
}

void Grafo::dfs(){
    
    for(Vertice* u : grafo){
        u->cor = WHITE;
    }

    num_componente = 0;
    tempo = 0;
    
    for(Vertice* v : grafo ){
        if(v->cor == WHITE){
            num_componente++;
            dfs_visita(v);
            //cout << '\n'; // Alteração p/ q3.
        }
    }
    
    //cout << num_componente << " connected components" << '\n'; // Alteração p/ q3.
    
}

void Grafo::dfs_visita(Vertice* v){
    
    v->cor = GRAY;
    tempo = tempo + 1;
    
    //cout << char(v->nome + 97) << ","; // Alteração p/ q3.
    
    for(Aresta* u : v->adj){
        if(grafo[u->destino]->cor == WHITE){
            dfs_visita(grafo[u->destino]);
        }
    }
    
    v->cor = BLACK;
    v->fim = tempo++;
    
}

void Grafo::prim(int v){

    priority_queue<Vertice*, vector<Vertice*>, comparator> Q;
    int src = 0;

    vector<float> key(num_vert, MAX);
    vector<int> parent(num_vert, -1);
    vector<bool> inMST(num_vert, false);

    Q.push(grafo[src]);
    key[src] = 0;

    float resp = 0;

    while(!Q.empty()){
        int u = Q.top()->nome;
        Q.pop();
        
        if(inMST[u] == true) continue;
        inMST[u] = true;
        
        for(auto x : grafo[u]->adj){
            int v = x->destino;
            float peso = x->peso;

            if(inMST[v] == true && key[v] > peso){
               key[v] = peso;
               Q.push(grafo[v]);
               parent[v] = u; 
            }

            if(inMST[v] == false && key[v] > peso){
                key[v] = peso;
                Q.push(grafo[v]);
                parent[v] = u;   
            }
        }
    }
    
    
    for(int i = 1; i < num_vert; ++i){
        //printf("%d - %d: %.2f\n", parent[i], i, key[i]);
        resp += key[i];
    }
    
    //cout << "Dist : " << resp << '\n';
    //printf("%s: %.2f\n","Dist", resp/100);
    printf("%.2f\n", resp/100);
    
}

void Grafo::fortemente_conectado(){
    stack<int> Stack;
    this->bolada = false;

    bool * visitado = new bool[num_vert];
    for(int i = 0; i < num_vert; i++){
        visitado[i] = false;
    }

    for(int i = 0; i < num_vert; i++){
        if(!visitado[i]){
            fill_order(i, visitado, Stack);
        }
    }

    Grafo gt = transposto();

    for(int i = 0; i < num_vert; i++){
        visitado[i] = false;
    }

    while(!Stack.empty()){
        int v = Stack.top();
        Stack.pop();

        if(!visitado[v]){
            gt.DFSUtil(v, visitado);
            cout << endl;
        }
    }

    cout << this->bolada << (this->bolada ? " | Bolada" : " | Nao Bolada") << '\n';
}

Grafo Grafo::transposto(){
    Grafo gt(num_vert);

    for(int i = 0; i < num_vert; i++){
        for(auto a : grafo[i]->adj){
            gt.add_aresta(a->destino, a->origem, a->peso);
        }
    }

    return gt;
}

void Grafo::DFSUtil(int v, bool visitado[]){
    visitado[v] = true;

    cout << v << " ";

    for(auto a : grafo[v]->adj){
        if(!visitado[a->destino]){
            this->bolada = true;
            DFSUtil(a->destino, visitado);
        }
    }
}

void Grafo::fill_order(int v, bool visitado[], stack<int> &Stack){
    visitado[v] = true;

    for(auto a : grafo[v]->adj){
        if(!visitado[a->destino]){
            fill_order(a->destino, visitado, Stack);
        }
    }
    Stack.push(v);
}

vector<int>Grafo::dijkstra(int src){
    priority_queue<Vertice*, vector<Vertice*>, comparator> Q;

    vector<int> dist(num_vert, MAX);
    vector<bool> f(num_vert, false);
    int parent[num_vert];

    Q.push(grafo[src]);
    dist[src] = 0;

    while(!Q.empty()){
        int u = Q.top()->nome;
        Q.pop();
        f[u] = true;

        for(auto a : grafo[u]-> adj){
            int v = a->destino;
            int preco = a->peso;

            if(f[v] == false && dist[v] > dist[u] + preco){
                dist[v] = dist[u] + preco;
                parent[v] = u;
                //cout << dist[v] << endl;
                Q.push(grafo[v]);
            }
        }
    }

    make_heap(dist.begin(), dist.end());

    return dist;

}

int Grafo::caminhos(int num_cam, int num_lug){
    bool *visited = new bool[num_vert];

    int *path = new int[num_vert];
    int path_index = 0;
    int preco = 0;
    int preco_final = 0;
    bool imp = false;
    
    for(int i = 0; i < num_vert; i++){
        visited[i] = false;
    }
    
    all_paths(0, num_vert - 1, imp, visited, path, path_index, preco, num_cam, num_lug, preco_final);
    return 1;
    //return all_paths(src, dst, visited, path, path_index);
}

void Grafo::all_paths(int src, int dst, bool imp, bool visited[], int path[], int &path_index, int &preco, int &num_cam, int num_lug, int &preco_final){
    visited[src] = true;
    path[path_index] = src;
    path_index++;
    priority_queue<int, vector<int>, greater<int>> menor_caminho;

    if(src == dst){
        menor_caminho.push(preco);
        while(!menor_caminho.empty() && num_cam > 0){ 
            preco_final += menor_caminho.top() * num_lug;
            menor_caminho.pop();
            num_cam--;
            if(num_cam == 0) cout << preco_final << endl;  
        }
    }else{
        for(auto a : grafo[src]->adj){
            if(!visited[a->destino]){
                preco = preco + a->peso;
                if( num_cam - num_lug > 0 && imp == false){
                    cout << "impossivel";
                    imp = true;
                } 
                all_paths(a->destino, dst, imp, visited, path, path_index, preco, num_cam, num_lug, preco_final);
            }
        }
    }

    preco = 0;
    path_index--;
    visited[src] = false;
}

bool Grafo::is_safe(int v, int color[], int c){
    for(int i = 0; i < grafo[v]->adj.size(); i++)
        if(grafo[v]->adj[i] && c == color[grafo[v]->adj[i]->destino]) 
            return false;
    return true;
}

bool Grafo::graph_colloring_util(int m, int color[], int v){
    
    if( v == num_vert) 
        return true;

    for(int c = 1; c <= m; c++){
        if(is_safe(v, color, c)){
            color[v] = c;

            if(graph_colloring_util(m, color, v+1) == true) 
                return true;

            color[v] = 0;       
        }
    }
    return false;
}

bool Grafo::graph_colloring(int m, int cores[]){
    int *color = new int[num_vert];
    for(int i = 0; i < num_vert; i++){
        color[i] = 0;
    }

    if(graph_colloring_util(m, color, 0) == false){
        return false;
    }
    
    print_collor_solution(color);
    for(int i = 0; i < num_vert; i++)
        if(cores[i] != color[i])
            return false;
    return true;
}

void Grafo::print_collor_solution(int color[]){
    for(int i =0; i < num_vert; i++){
        grafo[i]->cor = color[i];
        //cout << color[i] <<  " " ;
    }
    //cout << '\n';
}

void Grafo::add_aresta_color(int num_aresta){
    while(num_aresta > 0){
        for(int i = 0; i < num_vert; i++){
            for(int j = i; j < num_vert; j++){
                if(grafo[i]->cor != grafo[j]->cor && num_aresta > 0){
                    add_aresta(i, j, 1);
                    add_aresta(j, i, 1);
                    num_aresta--;
                }
            }
        }
    }
}

class TP{
public:
    void questao_um();
    void questao_dois();
    void questao_tres();
    void questao_quatro();
    void questao_cinco();
};

void TP::questao_um(){
    
    int num_exec;
    cin >> num_exec;
    int caso = 1;
    
    while(num_exec > 0){
        int num_vert, num_are;
        cin >> num_vert >> num_are;

        Grafo g(num_vert);
        
        while(num_are > 0){
            char n1, n2;
            cin >> n1 >> n2;
            
            int v1 = n1 - 97;
            int v2 = n2 - 97;
            
            g.add_aresta(v1, v2, 1);
            g.add_aresta(v2, v1, 1);
            
            num_are--;
        }
        
        cout << "Case #" << caso++ << ":\n" ;

        g.dfs();

        cout << '\n';
        
        num_exec--;
    }
    
}

void TP::questao_dois(){
    
    int num_exec;
    cin >> num_exec;
    
    while(num_exec > 0){
        int num_vert;
        cin >> num_vert;
        
        Grafo g(num_vert);
        
        vector<Ponto> p;
    
        for(int i = 0; i < num_vert; i++){
            int x, y;
            cin >> x >> y;
            p.push_back(Ponto(x, y));
        }

        for(int i = 0; i < num_vert; i++){
            for(int j = i; j < num_vert; j++){
                if( i != j ){
                    float dist = p[i].distancia(p[j].x, p[j].y);
                    g.add_aresta(i, j, dist);
                    g.add_aresta(j, i, dist);
                }
            }
        }
        
        g.print();
        
        g.prim(0);

        num_exec--;
    }
}

void TP::questao_tres(){
    int num_vert, num_are;

    cin >> num_vert >> num_are;

    Grafo g(num_vert);

    while(num_are > 0){
        int v1, v2;
        cin >> v1 >> v2;

        g.add_aresta(v1 - 1, v2 - 1, 1);

        num_are--;
    }

    g.fortemente_conectado();
} 

void TP::questao_quatro(){
    string line;
    int k = 0;
    
    while(true){
        getline(cin, line);
        if(!line.empty()){
            int num_vert, num_are;
            istringstream iss(line);
            vector<string> lido(istream_iterator<string>{iss},
                                istream_iterator<string>());
            num_vert = stoi(lido[0]);
            num_are = stoi(lido[1]);
            
            Grafo g(num_vert);

            while(num_are > 0){
                getline(cin, line);
                istringstream iss1(line);
                vector<string> lido1(istream_iterator<string>{iss1},
                                     istream_iterator<string>());

                g.add_aresta(stoi(lido1[0]) - 1, stoi(lido1[1]) - 1, stoi(lido1[2]));
                g.add_aresta(stoi(lido1[1]) - 1, stoi(lido1[0]) - 1, stoi(lido1[2]));

                num_are--;
            }

            getline(cin, line);
            istringstream iss2(line);
            vector<string> lido2(istream_iterator<string>{iss2},
                                 istream_iterator<string>());
            
            int num_ami = stoi(lido2[0]);
            int num_lug = stoi(lido2[1]);

            int preco_viagem = 0;

            cout << "Instancia " << ++k << endl << endl;

            //cout << num_ami/num_lug << endl;

            if(num_ami > num_lug){ g.caminhos(num_ami/num_lug, num_lug); }
            if(num_ami < num_lug){ g.caminhos( 1, num_ami); }

            cout << endl << endl << endl;

        }else{ break; }
    }
    

}

void TP::questao_cinco(){
    int num_exec;
    cin >> num_exec;

    while(num_exec > 0){
        int num_vert, num_are, num_add, num_cor;
        cin >> num_vert >> num_are >> num_add >> num_cor;

        Grafo g(num_vert);

        
        int *cores = new int[num_vert];

        for(int i = 0; i < num_vert; i++){
            int cor;
            cin >> cor;
            cores[i] = cor;
        }

        while(num_are >0){
            int v1, v2;
            cin >> v1 >> v2;
            g.add_aresta(v1 - 1, v2 - 1, 1);
            g.add_aresta(v2 - 1, v1 - 1, 1);

            num_are--;
        }

        //g.print();
        bool resp = false;

        g.graph_colloring(num_cor, cores);

        g.add_aresta_color(num_add);
        
        //g.print();
        resp = g.graph_colloring(num_cor, cores);

        cout << ((resp) ? "Y\n" : "N\n");
        cout << endl;
        num_exec--;
    }
}

int main(){
    TP tp;
    
    //tp.questao_um();      // 100% | 100%
    //tp.questao_dois();    // 100% | 100%
    //tp.questao_tres();    // 100% | 50%
    //tp.questao_quatro();  // 100% | 62,5%
    tp.questao_cinco();

    return 0;
}