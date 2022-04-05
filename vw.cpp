//Author: Yixiang ZHOU

#include<bits/stdc++.h>
using namespace std;

const long long max_flow = 17000; // 最大流量 
const long long inital_v = 22.25 * 1e8; // 初始水量 
const long long max_v = 25.85 * 1e8; // 最大水量 
const int divide = 100; // 多少个一组进行dp 
const long long d = 50; // 在0-maxflow枚举q时，q一次变化多少
const long long dt = 3600; // 时间间隔 

//map<long long, pair<long long, long long>> f[100];
//queue<long long> states;
int states_cnt; // 每次dp后统计新出现的状态数 
long long Q[300]; // 存放入库流量 
double value=0;

void work(int l, int r, long long &init) // 以init作为初始水量，dp计算Q[l]-Q[r] 
{
	map<long long, pair<double, long long>> f[300];
	// f[i][j].first 表示，第i时刻，水量为j时，目标函数f的值
	// f[i][j].second 表示，取得该函数值时，f[i-1]的状态值，用来输出dp结果 
	f[l-1][init] = {0,-1}; // 初始状态 
	queue<long long> states; // 存放每一次dp后新增加的状态，在原有的基础上继续扩展 
	states.push(init); 
	states_cnt = 1;
	for(int i=l; i<=r; i++)
	{
		cout << "Now :" << i << endl; // 当前在处理的时刻 
		cout << "states.size = " << states_cnt << endl; // 该时刻需要考虑的状态数
		// 每个时刻要计算的次数是 states_cnt * (maxflow / d) 级别的，由于map操作过多，常数很大 
		while(states_cnt--) // 从上一轮的状态递推当前 
		{
			long long last_v = states.front(); // 取出一个状态（水量），以此来递推 
			states.pop();
			for(long long q=max((Q[i]-(max_v-last_v)/dt),0ll); q<=max_flow; q+=d)
			// 枚举下泄流量q，起始值要保证水不溢出 
			{
				if(q-Q[i]<=0)
				// 考虑 入库流量 >= 出库流量 
				{
					double t1 = f[i-1][last_v].first+q*q*dt;
					bool ok = false;
					if(!f[i].count(last_v+(Q[i]-q)*dt)) ok = true;
					// 当前状态就是在last_v基础上加入新的 
					// 如果不存在当前状态就新建状态，否则与现有的状态比较 
					if(ok || f[i][last_v+(Q[i]-q)*dt].first >= t1)
					{
						f[i][last_v+(Q[i]-q)*dt] = {t1, last_v};
						if(ok)
						{
							states.push(last_v+(Q[i]-q)*dt);
						}
					}
				}
				else if(q-Q[i]>0)
				// 考虑出库流量 > 入库流量 
				{
					long long new_v = last_v - (q-Q[i])*dt;
					if(new_v < 0) break; // 水量低于一定值时就不用再枚举后面的q了 
					double tt = f[i-1][last_v].first;
					if(f[i].count(new_v)) 
					{
						if(f[i][new_v].first >= tt + q*q*dt)
						{
							f[i][new_v] = {tt + q*q*dt, last_v};
						}
					}
					else
					{
						f[i][new_v] = {tt + q*q*dt, last_v};
						states.push(new_v);
					}
				}
			}
		}
		states_cnt = states.size();
	}

	// 从最后一个状态递推DP时每步的状态 
	pair<double, long long> final = {2e306,0};
	long long v;
	for(auto iter : f[r])
	{	
		if((iter.second).first < final.first)
		{
			final = iter.second;
			v = iter.first;
		}
		double t1 = iter.second.first;
	}

	vector<long long> ans;
	ans.push_back(v);
	for(int i=r; i>=l+1; i--)
	{
		ans.push_back(final.second);
		final = f[i-1][final.second];
	}
	reverse(ans.begin(), ans.end());

	for(int i=0; i<ans.size(); i++)
		cout << i+l << " " << ans[i] << endl;	
	init = ans.back();
}

int main()
{
	int t;
	cin>>t;
	for(int i=1; i<=t; i++) cin>>Q[i];
	long long last = inital_v;
	int i = 1;
	while(1)
	{
		if(i+divide-1<=t)
		{
			work(i,i+divide-1,last);
			i += divide;
		}
		else
		{
			work(i,t,last);
			break;
		}
	}
	return 0;
}
