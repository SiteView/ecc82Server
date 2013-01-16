#ifndef		SVDB_BSTREE_
#define		SVDB_BSTREE_

#include <iostream>
#include "utiltype.h"
#include "exception.h"

using namespace std;



namespace svutil{
	template<class T>
	class treenode{
	public:
		treenode(const T &pe){
			m_element=pe;
			m_left=NULL;
			m_right=NULL;
			m_lflag=false;
			m_rflag=false;
		}
		treenode(T pe,treenode *left,treenode *right)
		{
			m_element=pe;
			m_left=left;
			m_right=right;
			m_lflag=false;
			m_rflag=false;
		}
		treenode()
		{
			//	m_element=NULL;
			m_left=NULL;
			m_right=NULL;
			m_lflag=false;
			m_rflag=false;
		}
		virtual ~treenode(){ ;}

		treenode<T> *Leftnode()
		{
			return m_left;
		}
		treenode<T> *rightnode()
		{
			return m_right;
		}

		void setleftnode(treenode<T> *leftnode)
		{
			m_left=leftnode;
		}

		void setrightnode(treenode<T> *rightnode)
		{
			m_right=rightnode;
		}

		void setelement(T element)
		{
			m_element=element;
		}

		T & getelement(void)
		{
			return m_element;
		}

		bool operator<(treenode<T> node)
		{
			return m_element<node.getelement();
		}

		bool operator>(treenode<T> node)
		{
			return m_element>node.getelement();
		}
		bool operator==(treenode<T> node)
		{
			return m_element==node.getelement();
		}

		bool operator>=(treenode<T> node)
		{
			return m_element>=node.getelement();
		}
		bool operator<=(treenode<T> node)
		{
			return m_element<=node.getelement();
		}

		///////////////////////////

		treenode<T> & operator=(const treenode<T> &node)
		{
			this->m_element=node.m_element;
			this->m_left=node.m_left;
			this->m_right=node.m_right;
			return *this;
		}

		treenode(const treenode<T> &node)
		{
			m_element=node.m_element;
			m_left=node.m_left;
			m_right=node.m_right;
		}

		bool getlflag(void)
		{
			return m_lflag;
		}
		bool getrflag(void){return m_rflag;}
		void setlflag(bool flag){m_lflag=flag;}
		void setrflag(bool flag){m_rflag=flag;}





	private:
		T m_element;
		treenode<T> *m_left,*m_right;
		bool m_lflag;
		bool m_rflag;

	};

	//template<class T>
	//typedef void (*FUNCVIST)(T *pnode);


	template<class T>
	class sorttree{
	public:
		sorttree(){m_root=NULL;m_isthread=false;}
		sorttree(treenode<T> *proot){m_root=proot;m_isthread=false;}
		virtual ~sorttree()
		{
			DeleteTree();
		}

		template<class T>
			static treenode<T> *CreateNode(const T &element)
		{
			treenode<T> *pNode=new treenode<T>(element);
			return pNode;
		};

		void MakeTreeByOrder(T*preorder,T*inorder,int count)
		{
			if(preorder==NULL||inorder==NULL||count<1)
				return;
			if(m_root!=NULL)
				DeleteTree();

			m_preindex=0;

			MakeTree(preorder,inorder,count,count,0,NULL);

		}

		void InitTree(){ if(m_root==NULL) m_root=new treenode<T>();}
		void DeleteTree(treenode<T> *proot=NULL)
		{
			if(proot==NULL)
				proot=m_root;
			if(proot==NULL)
				return;
			if(m_isthread)
				return;

			treenode<T> *ptemp=NULL;
			ptemp=proot->Leftnode();
			if(ptemp!=NULL)
				DeleteTree(ptemp);
			ptemp=proot->rightnode();
			if(ptemp!=NULL)
				DeleteTree(ptemp);
			delete proot;

		}
		void InsertNode(const T &element)
		{
			treenode<T> *pNode=CreateNode(element);
			if(pNode)
				InsertNode(pNode);
			else
				puts("CreateNode failed");
		}

		T&InsertNodeEx(const T &element)
		{
			treenode<T> *pNode=NULL;
			if((pNode=findnode(element))==NULL)
			    pNode=CreateNode(element);
			else
			{
				T& te=pNode->getelement();
				te=element;
				return te;
			}
			if(pNode)
			{
				InsertNode(pNode);
				return pNode->getelement();
			}else
				throw exception("Create node failed");
		}
		void InsertNode(treenode<T>* pnode,treenode<T> *after=NULL)
		{
			if(pnode==NULL)
				return;

			if(after==NULL)
				after=m_root;

			if(after==NULL)
			{
				m_root=pnode;
				return;
			}
			treenode<T> *ptemp=NULL;

			if(*pnode<*after)
			{
				ptemp=after->Leftnode();
				if(ptemp==NULL)
				{
					after->setleftnode(pnode);
					return;
				}else
					InsertNode(pnode,ptemp);
			}else if(*pnode>*after)
			{
				ptemp=after->rightnode();
				if(ptemp==NULL)
				{
					after->setrightnode(pnode);
					return;
				}else
					InsertNode(pnode,ptemp);
			}


			return ;
		}

		unsigned long GetNodesCount(void)
		{
			return GetSubTreeNodesCount(m_root);

		}

		unsigned long GetSubTreeNodesCount(treenode<T> *proot)
		{
			unsigned long count =0;
			CountNodes(proot,count);
			return count;
		}

		unsigned long GetLeafsCount()
		{
			return GetSubTreeLeafCount(m_root);
		}

		unsigned long GetSubTreeLeafCount(treenode<T> *proot)
		{
			unsigned long count = 0;
			CountLeafs(proot,count);
			return count;
		}

		treenode<T>* GetRootNode()
		{
			return m_root;
		}



		void RomoveNode(const T &element)
		{
			treenode<T> *ptemp=m_root;
			treenode<T> *pchild=NULL;
			treenode<T> *pnode=NULL;
			T temp;

			if(ptemp==NULL)
			{
				puts("empty tree");
				return;
			}
			BOOL isl=true;
			pchild=ptemp;
			while(true)
			{
				if(pchild==NULL)
				{
					puts("No the node");
					return;
				}
				temp=pchild->getelement();
				if(temp==element)
				{
					if(pchild->Leftnode()==NULL)
					{
						pnode=pchild->rightnode();

						if(ptemp!=pchild)
						{

							if(isl)
								ptemp->setleftnode(pnode);
							else
								ptemp->setrightnode(pnode);
						}else
							m_root=pnode;
						delete pchild;
						return;
					}else
					{

						pnode=pchild->Leftnode();
						if(pchild!=ptemp)
						{
							if(isl)
								ptemp->setleftnode(pnode);
							else
								ptemp->setrightnode(pnode);	
						}else
							m_root=pnode;

						if(pchild->rightnode()!=NULL)
						{
							while(pnode!=NULL)
							{
								ptemp=pnode;
								pnode=pnode->rightnode();
							}
							ptemp->setrightnode(pchild->rightnode());

						}
						delete pchild;
						return;

					}

				}else if(element<temp)
				{
					ptemp=pchild;
					pchild=ptemp->Leftnode();
					isl=true;
				}
				else
				{
					ptemp=pchild;
					pchild=ptemp->rightnode();
					isl=false;
				}


			}
		}

		bool inorder( void (vist)(const T &element),treenode<T> *proot=NULL)
		{
//			treenode<T> left=NULL,right=NULL;
			if(proot==NULL)
				proot=m_root;
			if(proot==NULL)
				return false;
			if(proot->Leftnode()!=NULL)
			{
				//	printf("(");
				inorder(vist,proot->Leftnode());
				//	printf(")");
			}
			vist(proot->getelement());
			if(proot->rightnode()!=NULL)
			{
				//		printf("(");
				inorder(vist,proot->rightnode());
				//		printf(")");
			}

			return true;
		}

		bool preorder( void (vist)(const T &element),treenode<T> *proot=NULL)
		{
			treenode<T> left=NULL,right=NULL;
			if(proot==NULL)
				proot=m_root;
			if(proot==NULL)
				return false;

			vist(proot->getelement());
			if(proot->Leftnode()!=NULL)
				preorder(vist,proot->Leftnode());
			if(proot->rightnode()!=NULL)
				preorder(vist,proot->rightnode());

			return true;


		}
		bool postorder( void (vist)(const T &element),treenode<T> *proot=NULL)
		{
			treenode<T> left=NULL,right=NULL;
			if(proot==NULL)
				proot=m_root;
			if(proot==NULL)
				return false;

			if(proot->Leftnode()!=NULL)
				postorder(vist,proot->Leftnode());
			if(proot->rightnode()!=NULL)
				postorder(vist,proot->rightnode());

			vist(proot->getelement());

			return true;


		}

		treenode<T> *findnode(const T &element,treenode<T> *pnode=NULL)
		{
			if(pnode==NULL)
				pnode=m_root;
			if(pnode==NULL)
				return NULL;
			if(element<pnode->getelement())
			{
				if(pnode->Leftnode()!=NULL)
					return findnode(element,pnode->Leftnode());
				else
					return NULL;
			}else if(element>pnode->getelement())
			{
				if(pnode->rightnode()!=NULL)
					return findnode(element,pnode->rightnode());
				else
					return NULL;
			}else if(element==pnode->getelement())
				return pnode;
			else
				return NULL;

		}

		treenode<T> *findnextnode(T &element,treenode<T> *pnode=NULL)
		{
			if(pnode==NULL)
				pnode=m_root;
			if(pnode==NULL)
				return NULL;
			bool has=false;

			return findnextnode(element,has,pnode);

		}

		void inthreadingtree(void)
		{
			if(m_isthread)
				return;

			inthreading();
			m_isthread=true;

		}

		void inorderbythreading(void (vist)(const T &element),treenode<T> *proot=NULL)
		{
			if(proot==NULL)
				proot=m_root;
			if(proot==NULL)
				return;

			if(!m_isthread)
				return;
			treenode<T> *pt=NULL;

			pt=proot;
			while(pt!=NULL)
			{

				while(!pt->getlflag()) pt=pt->Leftnode();
				vist(pt->getelement());
				while((pt->getrflag())&&(pt->rightnode()!=NULL))
				{
					pt=pt->rightnode();
					vist(pt->getelement());
				}

				pt=pt->rightnode();
			}



		}




	private:
		int m_preindex;
		bool m_isthread;
		treenode<T> *m_prenode;

		treenode<T> *findnextnode(T &element,bool &has,treenode<T> *pnode=NULL) const
		{
			if(pnode == NULL)
				return NULL;
			if(has)
				return pnode;

			if(element==pnode->getelement())
				has=true;

			treenode<T> *ptn=NULL;


			if(pnode->Leftnode()!=NULL)
			{
				ptn=findnextnode(element,has,pnode->Leftnode());
				if(ptn!=NULL)
					return ptn;
			}

			if(pnode->rightnode()!=NULL)
			{
				ptn=findnextnode(element,has,pnode->rightnode());
				if(ptn!=NULL)
					return ptn;
			}

			return NULL;



		}

		void CountLeafs(treenode<T> *proot,unsigned long &count)
		{
			if(proot==NULL)
				return;
			if((proot->Leftnode()==NULL)&&(proot->rightnode()==NULL))
			{
				count++;
				return;
			}

			if(proot->Leftnode()!=NULL)
				CountLeafs(proot->Leftnode(),count);
			if(proot->rightnode()!=NULL)
				CountLeafs(proot->rightnode(),count);
		}

		void CountNodes(treenode<T> *proot,unsigned long &count)
		{
			if(proot==NULL)
				return;
			count++;
			if(proot->Leftnode()!=NULL)
				CountNodes(proot->Leftnode(),count);

			if(proot->rightnode()!=NULL)
				CountNodes(proot->rightnode(),count);

		}


		void inthreading(treenode<T> *proot=NULL)
		{
			if(proot==NULL)
			{
				proot=m_root;
				if(proot==NULL)
					return;
				m_prenode=proot;
				if(proot->Leftnode()!=NULL)
				{
					inthreading(proot->Leftnode());
					return ;
				}
				else
				{
					proot->setlflag(true);
					return;
				}

				if(proot->rightnode()!=NULL)
				{
					inthreading(proot->rightnode());
					return ;
				}
				else
				{
					proot->setrflag(true);
					proot->setrightnode(proot);
					return ;
				}
			}
			treenode<T>*pre=NULL,*pt=NULL;

			pt=proot->Leftnode();

			if(pt!=NULL)
				inthreading(pt);
			else
			{
				proot->setlflag(true);
				proot->setleftnode(m_prenode);
			}



			pt=m_prenode->rightnode();
			if(pt==NULL)
			{
				m_prenode->setrflag(true);
				m_prenode->setrightnode(proot);
			}

			m_prenode=proot;

			pt=proot->rightnode();
			if(pt!=NULL)
				inthreading(pt);

			return;

		}

		void MakeTree(T *preorder,T*inorder,int totalcount,int right,int left=0,treenode<T> *proot=NULL)
		{
			if(m_preindex<0||m_preindex>=totalcount)
				return;

			int index=-1;

			if(proot!=NULL)
				index=FindIndex(proot->getelement(),inorder,totalcount);


			if(m_preindex==0)
			{
				treenode<T>*pt=CreateNode(preorder[m_preindex]);
				m_root=pt;
				m_preindex++;
				MakeTree(preorder,inorder,totalcount,right,left,m_root);
				return;
			}

			treenode<T> *pt=MakeNodeByOrder(preorder,inorder,totalcount,index,left);
			if(pt!=NULL)
			{
				proot->setleftnode(pt);
				MakeTree(preorder,inorder,totalcount,index,left,pt);
			}

			pt=MakeNodeByOrder(preorder,inorder,totalcount,right,index);
			if(pt!=NULL)
			{
				proot->setrightnode(pt);
				MakeTree(preorder,inorder,totalcount,right,index,pt);
			}

			return;



		}

		treenode<T>* MakeNodeByOrder(T *preorder,T*inorder,int totalcount,int right,int left=0)
		{
			T temp=preorder[m_preindex];
			int index=FindIndex(temp,inorder,totalcount);
			if(index<left||index>right)
				return NULL;
			m_preindex++;
			return CreateNode(temp);
		}


		int FindIndex(T element,T *order,int totalcount)
		{
			for(int i=0;i<totalcount;i++)
				if(element==order[i])
					return i;

			return -1;
		}
		treenode<T> *m_root;


	};

};

#endif;
