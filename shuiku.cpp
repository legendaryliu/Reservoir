#include<bits/stdc++.h>
using namespace std;

const long long max_flow = 17000; // ������� 
const long long inital_v = 22.25 * 1e8; // ��ʼˮ�� 
const long long max_v = 25.85 * 1e8; // ���ˮ�� 
const int divide = 100; // ���ٸ�һ�����dp 
const long long d = 50; // ��0-maxflowö��qʱ��qһ�α仯����
const long long dt = 3600; // ʱ���� 

//map<long long, pair<long long, long long>> f[100];
//queue<long long> states;
int states_cnt; // ÿ��dp��ͳ���³��ֵ�״̬�� 
long long Q[300]; // ���������� 
double value=0;

void work(int l, int r, long long &init) // ��init��Ϊ��ʼˮ����dp����Q[l]-Q[r] 
{
	map<long long, pair<double, long long>> f[300];
	// f[i][j].first ��ʾ����iʱ�̣�ˮ��Ϊjʱ��Ŀ�꺯��f��ֵ
	// f[i][j].second ��ʾ��ȡ�øú���ֵʱ��f[i-1]��״ֵ̬���������dp��� 
	f[l-1][init] = {0,-1}; // ��ʼ״̬ 
	queue<long long> states; // ���ÿһ��dp�������ӵ�״̬����ԭ�еĻ����ϼ�����չ 
	states.push(init); 
	states_cnt = 1;
	for(int i=l; i<=r; i++)
	{
		cout << "Now :" << i << endl; // ��ǰ�ڴ����ʱ�� 
		cout << "states.size = " << states_cnt << endl; // ��ʱ����Ҫ���ǵ�״̬��
		// ÿ��ʱ��Ҫ����Ĵ����� states_cnt * (maxflow / d) ����ģ�����map�������࣬�����ܴ� 
		while(states_cnt--) // ����һ�ֵ�״̬���Ƶ�ǰ 
		{
			long long last_v = states.front(); // ȡ��һ��״̬��ˮ�������Դ������� 
			states.pop();
			for(long long q=max((Q[i]-(max_v-last_v)/dt),0ll); q<=max_flow; q+=d)
			// ö����й����q����ʼֵҪ��֤ˮ����� 
			{
				if(q-Q[i]<=0)
				// ���� ������� >= �������� 
				{
					double t1 = f[i-1][last_v].first+q*q*dt;
					bool ok = false;
					if(!f[i].count(last_v+(Q[i]-q)*dt)) ok = true;
					// ��ǰ״̬������last_v�����ϼ����µ� 
					// ��������ڵ�ǰ״̬���½�״̬�����������е�״̬�Ƚ� 
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
				// ���ǳ������� > ������� 
				{
					long long new_v = last_v - (q-Q[i])*dt;
					if(new_v < 0) break; // ˮ������һ��ֵʱ�Ͳ�����ö�ٺ����q�� 
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

	// �����һ��״̬����DPʱÿ����״̬ 
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
