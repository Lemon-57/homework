#include <iostream>
#include<queue>
#include<string.h>
#include<algorithm>
using namespace std;
const int maxn = 210;
char mp[maxn][maxn];
bool visited[maxn][maxn];
int dx[4] = {0, 1, -1, 0};
int dy[4] = {-1, 0, 0, 1};
int n, m;//地图大小
int sx, sy;//起点
int ex, ey;//终点


struct Node//保存x，y坐标和当前所用步数
{
    int x, y;
    int step;
}node;

bool operator < (Node a, Node b){//定义结构体类型的优先队列的优先级，step小的优先
    return a.step > b.step;
}

void getMap(int n, int m){//输入地图
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++){
            cin >> mp[i][j];
            if(mp[i][j] == 'r'){//记录 'r'代表天使的每一个朋友的位置
                sx = i;
                sy = j;
            }
            if(mp[i][j] == 'a'){//记录'a'代表天使的位置
                ex = i;
                ey = j;
            }
        }

    }
}

bool judge(int x, int y){//判断x，y是否能到达 
    //'#'不能走
    if(x >= 1 && x <= n && y>= 1 && y <= m && !visited[x][y] && mp[x][y] != '#'){
        return true;
    }
    return false;
}

int bfs(int x, int y){
    memset(visited, 0, sizeof(visited));
    Node a, b;
    a.x = x;
    a.y = y;
    a.step = 0;
    priority_queue<Node>q;
    q.push(a);
    while (!q.empty())
    {
        b = q.top();
        q.pop();
        for(int i = 0; i < 4; i++){
            int nextx = b.x + dx[i];
            int nexty = b.y + dy[i];
            if(judge(nextx, nexty)){
                visited[nextx][nexty] = 1;
                a.x = nextx;
                a.y = nexty;
                if(mp[nextx][nexty] == 'x'){//走到守卫+杀死守卫 需要2步
                    a.step = b.step + 2;
                    q.push(a);
                }
                else{
                    a.step = b.step + 1;//非守卫只需要走一步
                    q.push(a);
                    if(nextx == ex && nexty == ey){//找到就返回，不用接着找了
                        return a.step;
                    }
                }
            }
        }
    }
    return -1;//没找到
}

int main()
{
    while (cin >> n >> m)
    {
        getMap(n, m);
        int ans = bfs(sx, sy);
        if(ans == -1){
            cout << "Poor ANGEL has to stay in the prison all his life." <<endl;
        }
        else{
            cout << ans << endl;
        }
    }
    
    return 0;
}

