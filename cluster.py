#!/usr/bin/env python
# coding: utf-8

# In[117]:


from firebase import firebase 
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
import pandas as pd


# In[57]:


cred2 = credentials.Certificate(r"C:\Users\Pranav A\Downloads\potholes-cde20-firebase-adminsdk-8qwjh-e0dd8235ae.json")
firebase_admin.initialize_app(cred2, {
    'databaseURL': 'https://potholes-cde20-default-rtdb.firebaseio.com/'
})


# In[127]:


ref = db.reference('/users')
print(ref.get())


# In[128]:


d=ref.get()
for i in d:
    print(d[i])


# In[131]:


new_data={"lats":[], "longs":[]}
for i in d.values():
    new_data["lats"].append(i.split(',')[0])
    new_data["longs"].append(i.split(',')[1])


# In[ ]:


for i in range(len(new_data["lats"])){
    
}


# In[132]:


new_data["lats"]


# In[173]:


longitudes=[]
latitudes=[]
avlong=0
avlat=0
for i in range(0,len(new_data["longs"])-1):
    avlong=avlong+(((float(new_data["longs"][i]))*10**6)//10**3);
    avlat=avlat+(((float(new_data["lats"][i]))*10**6)//10**3);
    longitudes.append((((float(new_data["longs"][i]))*(10**6))%(10**3)))
    latitudes.append((((float(new_data["lats"][i]))*(10**6))%(10**3)))
avlat=avlat/(len(new_data["longs"])-1)
avlat=int(avlat)/10**3
avlat


# In[174]:


avlong=avlong/(len(new_data["longs"])-1)
avlong=int(avlong)/10**3
avlong


# In[164]:


((float(new_data["lats"][3]))*10**6)//10**3


# In[157]:


avlong/len(new_data["longs"])


# In[134]:


latitudes


# # longitudes=[]
# latitudes=[]
# for i in range(0,len(new_data["longs"])-1):
#     if(i%3==0):
#         longitudes.append(new_data["longs"][i])
#         latitudes.append(new_data["lats"][i])
#         
# longitudes 
# latitudes

# In[135]:


new_dict={'lats':latitudes, 'longs':longitudes}


# In[136]:


df=pd.DataFrame.from_dict(new_dict)


# In[137]:


df.head()


# In[138]:


import matplotlib.pyplot as plt
plt.scatter(df['lats'], df['longs'])
plt.xscale('linear')


# In[140]:


from sklearn.cluster import KMeans
kmeans=KMeans(n_clusters=5)
y_pred=kmeans.fit_predict(df[['lats', 'longs']])
y_pred


# In[142]:


df['cluster']=y_pred
df1 = df[df.cluster==0]
df2 = df[df.cluster==1]
df3 = df[df.cluster==2]
df4 = df[df.cluster==3]
df5 = df[df.cluster==4]
plt.scatter(df1['lats'],df1['longs'],color='green')
plt.scatter(df2['lats'],df2['longs'],color='red')
plt.scatter(df3['lats'],df3['longs'],color='black')
plt.scatter(df4['lats'],df4['longs'],color='blue')
plt.scatter(df5['lats'],df5['longs'],color='orange')
plt.scatter(kmeans.cluster_centers_[:,0],kmeans.cluster_centers_[:,1],color='purple',marker='*',label='centroid')
plt.xlabel('lattitude')
plt.ylabel('longitude')
plt.legend()


# In[196]:


x=kmeans.cluster_centers_[:,0]
y=kmeans.cluster_centers_[:,1]
print(x,y)
long1=[]
lat1=[]
for i in range(0,len(x)):
    long1.append(((y[i]/(10**6))))
    lat1.append(((x[i]/(10**6))))
long1


# In[192]:


lat1


# In[193]:


clat=[]
clong=[]
for i in range(len(x)):
    clat.append(avlat+lat1[i])
    clong.append(avlong+long1[i])


# In[194]:


clat


# In[195]:


clong


# In[202]:


data={"centroid lat":clat, "centroid long":clong, "ppc":no_of_potholes}
data["centroid lat"]


# In[203]:


data["centroid long"]


# In[204]:


data["ppc"]


# In[207]:


import json


# In[209]:


json_object = json.dumps(data, indent=4)


# In[201]:


no_of_potholes=[df1.shape[0],df2.shape[0],df3.shape[0],df4.shape[0],df5.shape[0]]


# In[210]:


ref1=db.reference('/cluster')
ref1.set(json_object)

