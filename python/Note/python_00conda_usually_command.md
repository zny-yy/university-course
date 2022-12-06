# conda常用命令

## 创建虚拟环境
```python
#envname是环境名称
conda create --name envname  
conda create --name envname python=3.5 #创建指定python版本
conda create --name envname python=3.5 numpy dcipy # 创建指定python版本下包含某些包
```
## 激活/使用/进入某个虚拟环境
``` python
#envname是环境名称
conda activate  envname
```

## 退出当前环境
``` python
conda deactivate
```

## 复制某个虚拟环境
``` python
#new_envname是新环境名称,old_envname是旧环境名称
conda  create  --name  new_envname  --clone  old_envname
```

## 删除某个环境
``` python
#envname是环境名称
conda  remove  --name  envname  --all
```
## 查看当前所有环境
``` python
conda  info  --envs   
#或者  
conda  env  list
```

## 查看当前环境下的所有安装包
``` python
#envname是环境名称
conda  list  #需进入该虚拟环境
conda  list  -n  envname
```
## 安装或卸载包(进入虚拟环境之后）
``` python
conda  install  xxx
conda  install  xxx=版本号  # 指定版本号
conda  install  xxx -i 源名称或链接 # 指定下载源
conda  uninstall  xxx
```

## 分享虚拟环境
``` python
conda env export > environment.yml  # 导出当前虚拟环境
conda env create -f environment.yml  # 创建保存的虚拟环境
```

## 源服务器管理
``` python
#conda当前的源设置在$HOME/.condarc中，可通过文本查看器查看或者使用命令查看。
conda config --show-sources #查看当前使用源
conda config --remove channels 源名称或链接 #删除指定源
conda config --add channels 源名称或链接 #添加指定源
#例如：
conda config --remove channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/conda-forge/
```

## 升级
``` python
#升级Anaconda需先升级conda：
conda  update  conda
conda  update  anaconda
```

## 卸载
``` python
rm  -rf  anaconda
```

## 批量导出虚拟环境中的所有组件
``` python 
conda list -e > requirements.txt  # 导出
conda install --yes --file requirements.txt  # 安装
```
## pip批量导出环境中的所有组件
``` python
pip freeze > requirements.txt
pip install -r requirements.txt
```

