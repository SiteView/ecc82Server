#ifndef	SVDB_HASHTABLE_
#define SVDB_HASHTABLE_
#include <string>
#include <vector>
#include "bstree.h"
#include "strkey.h"
#include "svdbtype.h"

using namespace std;

namespace svutil{

	//typedef unsigned int	S_UINT;

	template<class KEY_,class VALUE_>
	class hashelement{
		friend static void  printelement(const hashelement<svutil::word,int> &element);
	public:
		hashelement()
		{
		}
		hashelement(const KEY_ &key)
		{
			m_key=key;
		}
		hashelement(const KEY_ &key,const VALUE_ &value)
		{
			m_key=key;
			m_value=value;
		}
		hashelement(const hashelement<KEY_,VALUE_> &he)
		{
			m_key=he.m_key;
			m_value=he.value;
		}
		~hashelement()
		{
		}
		KEY_ &getkey()
		{
			return m_key;
		}
		VALUE_& getvalue()
		{
			return m_value;
		}

		void setkey(const KEY_ &key)
		{
			m_key=key;

		}

		void setvalue(const VALUE_ &value)
		{
			m_value=value;
		}
		bool operator<(const hashelement<KEY_,VALUE_>&element) const 
		{
			return m_key<element.m_key;
		}
		
		bool operator==(const hashelement<KEY_,VALUE_> &element) const
		{
			return m_key==element.m_key;
		}
		bool operator>(const hashelement<KEY_,VALUE_> &element) const
		{
			return m_key>element.m_key;
		}
		hashelement<KEY_,VALUE_> & operator=(const hashelement<KEY_,VALUE_> &element)
		{
			this->m_key=element.m_key;
			this->m_value=element.m_value;
			return *this;
		}



	private:
		KEY_ m_key;
		VALUE_ m_value;
		
	};

		static void  printelement(const hashelement<svutil::word,int> &element)
		{

			printf("other:%s\n",(char *)element.m_key);
		}


	template<class KEY_,class VALUE_>
	class hashnode{
	public:
		hashnode(const KEY_ &key,const VALUE_ &value):m_element(key,value)
		{
			m_eraseflag=false;
		}
		hashelement<KEY_,VALUE_> &getelement(void)
		{
			if(!m_eraseflag)
		    	return m_element;
			else
			{
				svutil::treenode<hashelement<KEY_,VALUE_> > *pnode=NULL;
				pnode=m_otherelements.GetRootNode();
				return pnode->getelement();
			}
		}
		~hashnode(){;};
	     VALUE_ &getvalue(void)
		{
			return m_element.getvalue();
		}

		const KEY_ &getkey(void) const
		{
			return m_element.getkey();
		}

	/*	void pushnext(const KEY_ &key,const VALUE_ &value)
		{
			hashelement<KEY_,VALUE_> element(key,value);
			m_otherelements.InsertNode(element);
		}*/

		hashelement<KEY_,VALUE_> &insert(const KEY_ &key,const VALUE_ &value)
		{
			if(!m_eraseflag)
			{
				KEY_ &tk=m_element.getkey();
				if(tk==key)
				{
					m_element.setvalue(value);
					return m_element;
				}
			}else
			{
				hashelement<KEY_,VALUE_> *pele=NULL;
				if(!find(key,&pele))
				{
					m_element.setkey(key);
					m_element.setvalue(value);
					m_eraseflag=false;
					return m_element;
				}
			}

			hashelement<KEY_,VALUE_> element(key,value);
			return m_otherelements.InsertNodeEx(element);
		}

		bool find(const KEY_ &key,hashelement<KEY_,VALUE_>**pelement)
		{
		
			hashelement<KEY_,VALUE_> element(key);
			if(!m_eraseflag)
			{
				if(m_element==element)
				{
					*pelement=&m_element;
					return true;
				}
			}
			svutil::treenode<hashelement<KEY_,VALUE_> > *pnode=m_otherelements.findnode(element);
			if(pnode==NULL)
				return false;
			*pelement=&(pnode->getelement());
			return true;

		}
		bool find(const KEY_ &key,VALUE_ **pvalue)
		{
			if(!m_eraseflag)
			{
				if(m_element.getkey()==key)
				{
					VALUE_&tt=m_element.getvalue();
					*pvalue=&tt;
					return true;
				}
			}

			hashelement<KEY_,VALUE_> *pet=NULL;
			if(find(key,&pet))
			{
				*pvalue=&(pet->getvalue());
				return true;
			}

			return false;
		}

		bool hasother()
		{
			return (m_otherelements.GetNodesCount()>0);
		}

		bool findnext(hashelement<KEY_,VALUE_> &ele,hashelement<KEY_,VALUE_> **pelement)
		{
			if(!hasother())
				return false;
			svutil::treenode<hashelement<KEY_,VALUE_> > *pnode=NULL;
			if(!m_eraseflag)
			{
				if(ele==m_element)
					pnode=m_otherelements.GetRootNode();
				else
					pnode=m_otherelements.findnextnode(ele);
			}
			else
				pnode=m_otherelements.findnextnode(ele);

			if(pnode==NULL)
					return false;
			*pelement=&(pnode->getelement());

			return true;       

		}

		S_UINT length()
		{
			if(m_eraseflag)
				return m_otherelements.GetNodesCount();
			else
				return m_otherelements.GetNodesCount()+1;
		}


	    bool erase(const KEY_ &key)
		{
			if(key==m_element.getkey())
			{
				m_eraseflag=true;
				if(hasother())
					return true;
				else
					return false;
			}else
			{
				hashelement<KEY_,VALUE_> element(key);
				m_otherelements.RomoveNode(element);
				if(!hasother())
				{
					if(m_eraseflag)
						return false;
				}
				return true;

			}

			return true;

		}


		void Debug_print(void)
		{
			printf("main:%s\n",(char *)m_element.getkey());
			unsigned long  count=m_otherelements.GetNodesCount();
			printf("same code count :%d\n",count);
			if(count>0)
				m_otherelements.inorder(&printelement);


		}

	private:
		hashelement<KEY_,VALUE_> m_element;
		svutil::sorttree<hashelement<KEY_,VALUE_> > m_otherelements;
		bool m_eraseflag;

	};

	template <class TY_>
	class hiterator{
	public:
		hiterator()
		{
			m_pos=-1;
			m_element=NULL;
			m_intree=false;
		}
		hiterator(const hiterator<TY_> &hi)
		{
			m_pos=hi.m_pos;
			m_element=hi.m_element;
			m_intree=hi.m_intree;
		}
		hiterator<TY_> & operator=(const hiterator<TY_> &hi)
		{
			m_pos=hi.m_pos;
			m_element=hi.m_element;
			m_intree=hi.m_intree;

			return *this;
		}
	protected:
		int	m_pos;
		TY_		*m_element;
		bool	m_intree;
	};

	template<class KEY_,class VALUE_>
	class hashtable{
		typedef hashnode<KEY_,VALUE_> HNODE;
		typedef hashelement<KEY_,VALUE_> HELEMENT;
		typedef hashtable<KEY_,VALUE_> MYT;
		friend class iteraotr;
	public:

		enum{ initsize=13 };

		hashtable()
		{
			m_nodes.resize(initsize,NULL);
			m_tablesize=initsize;
		}

		hashtable(int size)
		{
			if(size>0)
			   m_tablesize=size;
			else
			  m_tablesize=initsize;

			m_nodes.resize(m_tablesize,NULL);

		}
		~hashtable(){
			clearnodes();
		};

		VALUE_ &insert(const KEY_ &key,const VALUE_ &value)
		{
			if(m_tablesize<=0)
			{
				m_tablesize=initsize;
				m_nodes.resize(m_tablesize,NULL);
			}

			S_UINT nHash=0;
			nHash=HashKey((char *) key)%m_tablesize;

			HNODE*pnode=m_nodes[nHash];
			if(pnode==NULL)
			{
				m_nodes[nHash]=new HNODE(key,value);
				return m_nodes[nHash]->getvalue();
			}
			else
				return pnode->insert(key,value).getvalue();
			
		}

	    S_UINT getvaluecount()
		{
			S_UINT totalcount=0;
			S_UINT ncount = m_nodes.size();
			for(int i=0;i<ncount;i++)
			{
				if(m_nodes[i]!=NULL)
					totalcount++;
					
			}
			return totalcount;

		}

		S_UINT size()
		{
			S_UINT totalcount=0;
			S_UINT ncount=(S_UINT)m_nodes.size();
			for(S_UINT i=0;i<ncount;i++)
			{
				if(m_nodes[i]!=NULL)
				{
					totalcount+=m_nodes[i]->length();
				}
			}

			return totalcount;
		}

		S_UINT spacesize()
		{
			return m_tablesize;
		}

		VALUE_ * find(const KEY_ &key)
		{
			if(m_tablesize<=0)
				return NULL;

			S_UINT nHash=HashKey((char *)key)%m_tablesize;

			HNODE *pnode=m_nodes[nHash];
			if(pnode==NULL)
				return NULL;
			else
			{
				VALUE_ *pv=NULL;
			    if(pnode->find(key,&pv))
				{
					return pv;
				}else
					return NULL;
			}

		}

		VALUE_& operator[](const KEY_ &key)
		{
			VALUE_* pv=NULL;
			if((pv=find(key))!=NULL)
			{
				return *pv;
			}else
			{
				VALUE_ tv;
				return insert(key,tv);
			}

		}

		bool erase(const KEY_ &key)
		{
			if(m_tablesize<=0)
				return false;

			S_UINT nHash=HashKey((char *)key)%m_tablesize;

			HNODE *pnode=m_nodes[nHash];
			if(pnode==NULL)
				return false;
			else
			{
				VALUE_ *pv=NULL;
				if(!pnode->find(key,&pv))
					return false;
				else
				{
					if(!pnode->erase(key))
					{
						delete pnode;
						m_nodes[nHash]=NULL;
					}
					return true;
				}
			}


		}

		void Debug_print(void)
		{
			S_UINT ncount = m_nodes.size();
			for(int i=0;i<ncount;i++)
			{
				if(m_nodes[i]!=NULL)
					m_nodes[i]->Debug_print();
			}


		}

		class iterator : public svutil::hiterator<HELEMENT >
		{
			friend class hashtable<KEY_,VALUE_>;
		public:
			iterator()
			{
				m_pos=-1;
				m_element=NULL;
				m_intree=false;
			}
			iterator(const iterator &hi)
			{
				m_pos=hi.m_pos;
				m_element=hi.m_element;
				m_intree=hi.m_intree;
			}
			iterator & operator=(const iterator &hi)
			{
				m_pos=hi.m_pos;
				m_element=hi.m_element;
				m_intree=hi.m_intree;

				return *this;
			}

			HELEMENT & operator*() const
			{
				return (*m_element);

			}
			void clear(void)
			{
				m_pos=-1;
				m_element=NULL;
				m_intree=false;

			}

		};

	/*	bool findfirst(iterator &it)
		{
			S_UINT ncount=m_nodes.size();
			for(int i=0;i<ncount;i++)
			{
				if(m_nodes[i]!=NULL)
				{
					it.m_pos=i;
					it.m_element=&(m_nodes[i]->getelement());
					it.m_intree=m_nodes[i]->hasother();

					return true;

				}
			}
			return false;

		}*/

		bool findnext(iterator &it)
		{
			S_UINT ncount=(S_UINT)m_nodes.size();
			if(it.m_pos>-1)
			{
				if(it.m_pos>=ncount)
					return false;

				hashelement<KEY_,VALUE_> *pelement=NULL;
				if(m_nodes[it.m_pos]==NULL)
					return false;
				if(m_nodes[it.m_pos]->findnext(*(it.m_element),&pelement))
				{
					it.m_element=pelement;
					return true;
				}
			}

			for(int i=it.m_pos+1;i<ncount;i++)
			{
				if(m_nodes[i]!=NULL)
				{
					it.m_pos=i;
					it.m_element=&(m_nodes[i]->getelement());

					return true;
				}
			}

			return false;

		}

		void resetsize(S_UINT len)
		{
			if(m_tablesize==len)
				return;
			m_nodes.resize(len,NULL);
			m_tablesize=len;
		}

		void clear()
		{
			clearnodes();
		}

	protected:



		void clearnodes()
		{
			S_UINT ncount = m_tablesize;
			for(S_UINT i=0;i<ncount;i++)
			{
				if(m_nodes[i]!=NULL)
					delete m_nodes[i];
				m_nodes[i]=NULL;
			}

		}



	private:

		unsigned int HashKey(const char * key)
		{
			unsigned int nHash=0;
			while (*key)
				nHash = (nHash<<5) + nHash + *key++;
			return nHash;
		}

		unsigned long HashKey2(const char * key)
		{
			unsigned long h = 0; 
			for ( ; *key; ++key)
				h = 5*h + *key;

			return h;

		}

	//	HNODE * m_nodes[initsize];
	//	HNODE * m_hugenodes;

		std::vector<HNODE *> m_nodes;
		S_UINT m_tablesize;
	};

};

#endif