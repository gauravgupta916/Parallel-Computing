#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


# In[2]:


df = pd.read_csv('final_output.txt', sep=" ", header=None)
df = df[1:]
df.columns = ["n", "threads", "=", "serial","pthread","omp"]
df = df.drop(["="], axis = 1)


# In[3]:


#when all threads are used
#x-axis matrix size(thread=6)[100(6),200(6),300(6),...,1000(6)]
#y-axis execution time

df_replica =  df[df['threads']=='6']
x_dash = df_replica['n'].astype(int).values
y_s = df_replica['serial'].astype(int).values
y_p = df_replica['pthread'].astype(int).values
y_o = df_replica['omp'].astype(int).values
plt.plot(x_dash,y_s,c='red',marker='^',label='serial')
plt.plot(x_dash,y_p,c='green',marker='o',label='pthread')
plt.plot(x_dash,y_o,c='blue',marker='*',label='OpenMP')
plt.xlabel('matrix size',fontsize=20)
plt.ylabel('time',fontsize=20)
plt.legend()
plt.title('Matrix size vs Time graph')


# In[4]:


# Thread vs Time graph
fig_, axs = plt.subplots(4, 3,figsize=(15,15))
N = 100
y_ = df['threads'].unique().astype(int)
for i in range(4):
    for j in range(3):
        if(i==3 and j>0):
            continue
        axs[i,j].plot(y_,df[df['n']==str(N)]['serial'].astype(int).values,'tab:red',label='serial')
        axs[i,j].plot(y_,df[df['n']==str(N)]['pthread'].astype(int).values,'tab:green',label='pthread')
        axs[i,j].plot(y_,df[df['n']==str(N)]['omp'].astype(int).values,'tab:blue',label='OpenMP')
        axs[i,j].set_title('N = '+str(N))
        if i == 0 and j==0:
            axs[i,j].legend()
        N = N + 100

for ax in axs.flat:
    ax.set(xlabel='thread', ylabel='time')

for ax in axs.flat:
    ax.label_outer()

fig_.suptitle('2D graph for different values of matrix size vs Time', fontsize=16)


# Thread vs Speed up graph
fig_1, axs1 = plt.subplots(4, 3,figsize=(15,15))
N1 = 100
y_1 = df['threads'].unique().astype(int)
for i in range(4):
    for j in range(3):
        if i==3 and j>0 :
            continue
        time_serial = df[df['n']==str(N1)]['serial'].unique().astype(int)
        time_pthread = df[df['n']==str(N1)]['pthread'].astype(int)
        time_omp = df[df['n']==str(N1)]['omp'].astype(int)
        speed_up_pthread = [time_serial/k for k in time_pthread ]
        speed_up_omp = [time_serial/k for k in time_omp]
        axs1[i,j].plot(y_1,speed_up_pthread,'tab:green',label='pthread')
        axs1[i,j].plot(y_1,speed_up_omp,'tab:blue',label='OpenMP')
        axs1[i,j].set_title('N = '+str(N1))    
        axs1[i,j].legend()
        N1 = N1 + 100

for ax in axs1.flat:
    ax.set(xlabel='thread', ylabel='time')

for ax in axs1.flat:
    ax.label_outer()

fig_1.suptitle('Speed up analysis', fontsize=16)


# In[5]:


# x - matrix size
# y - thread
# z - time in microseconds
x = df['n'].astype(int).values
y = df['threads'].astype(int).values
z1 = df['serial'].astype(int).values
z2 = df['pthread'].astype(int).values
z3 = df['omp'].astype(int).values
fig = plt.figure()
fig.set_figwidth(20)
fig.set_figheight(20)
ax = plt.axes(projection='3d')
ax.scatter3D(x, y, z1, c='red', s=50,label='serial');
ax.scatter3D(x, y, z2, c='green',s=50,label='pthread');
ax.scatter3D(x, y, z3, c='blue',s=50,label='OpenMP');
ax.plot3D(x, y, z1, c='red');
ax.plot3D(x, y, z2, c='green');
ax.plot3D(x, y, z3, c='blue');
ax.set_xlabel('matrix size',fontsize=20)
ax.set_ylabel('threads',fontsize=20)
ax.set_zlabel('time',fontsize=18)
plt.legend()
plt.title('Matrix size vs Threads vs Time graph')
plt.show()


# In[6]:









