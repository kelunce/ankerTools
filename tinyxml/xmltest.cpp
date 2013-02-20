/*
   Test program for TinyXML.
*/


#include "tinyxml.h"
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;






//
// This file demonstrates some basic functionality of TinyXml.
// Note that the example is very contrived. It presumes you know
// what is in the XML file. But it does test the basic operations,
// and show how to add and remove nodes.
//

bool GetNodePointerByName(TiXmlElement* pRootEle, const char* strNodeName,TiXmlElement* &destNode);

int main()
{

	//
	// We start with the 'demoStart' todo list. Process it. And
	// should hopefully end up with the todo list as illustrated.
	//

    char  pBuff[2048] ={0};
    strcat(pBuff, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
    strcat(pBuff, "<!-- edited with XMLSpy v2010 rel. 3 sp1 (http://www.altova.com) by MESMERiZE (MSM) -->");
    strcat(pBuff, "<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" elementFormDefault=\"qualified\" attributeFormDefault=\"unqualified\">");
    strcat(pBuff, "<xs:annotation>");
    strcat(pBuff, "<xs:documentation>通知消息</xs:documentation>");
    strcat(pBuff, "</xs:annotation>");
    strcat(pBuff, "<xs:complexType name=\"ConsumeResultPost\">");
    strcat(pBuff, "<xs:element name=\"userId\" value=\"20132222\"/>");
    strcat(pBuff, "<xs:element name=\"contentId\" value=\"cid\"/>");
    strcat(pBuff, "<xs:element name=\"consumeCode\" value=\"cc6666\"/>");
    strcat(pBuff, "<xs:element name=\"cpid\" value=\"cpid0\"/>");
    strcat(pBuff, "<xs:element name=\"hRet\" value=\"2\"/>");
    strcat(pBuff, "<xs:element name=\"versionId\" value=\"2_0_0\"/>");
    strcat(pBuff, "<xs:element name=\"cpparam\" value=\"cpppp\"/>");
    strcat(pBuff, "<xs:element name=\"packageID\" value=\"29\"/>");
    strcat(pBuff, "</xs:complexType>");
    strcat(pBuff, "</xs:schema>");

    // 把字符串格式转换为TiXmlDocument
    TiXmlDocument doc;
    doc.Parse(pBuff);   

    TiXmlElement* pRoot = doc.RootElement();
    if(pRoot)
    {
        TiXmlElement *pEle = 0;
        GetNodePointerByName(pRoot,"xs:complexType",pEle);
        if(pEle)
        {
            const char * pAttr = pEle->Attribute("name");
            if(pAttr && 0 == strcmp(pAttr,"ConsumeResultPost"))
            {
                TiXmlNode* pNode = pEle->FirstChild();
                while(pNode)
                {
                    TiXmlElement* pXmlEle = pNode->ToElement();
                    if(pXmlEle)
                    {
                        if(pXmlEle->Attribute("name") && pXmlEle->Attribute("value"))
                        {
                            cout<<pXmlEle->Attribute("name")<<"    ";
                            cout<<pXmlEle->Attribute("value")<<endl;                            
                        }
                    }
                    //cout<<pNode->Value()<<endl;
                    pNode = pNode->NextSibling();
                }
            }
            // 修改XML
            pEle->Clear();
            TiXmlElement node("xs:element");
            node.SetAttribute("name","hRet");
            node.SetAttribute("value","0");
            pEle->InsertEndChild(node);
            TiXmlElement node1("xs:element");
            node1.SetAttribute("name","message");
            node1.SetAttribute("value","Suc");
            pEle->InsertEndChild(node1);

            // 序列化为字符串格式
            TiXmlPrinter printer;
            doc.Accept(&printer);
            cout<<printer.Str()<<endl;
            // 字符串大小字节
            cout<<strlen(printer.CStr())<<" ---"<<printer.Size()<<endl;
            
            //doc.Print();
        }

    //        // 遍历一级
    //        TiXmlNode* pNode = doc.FirstChild();
    //        cout<<pNode->Value()<<endl;
    //        while (true)
    //        {
    //            pNode = doc.IterateChildren(pNode);
    //            if(pNode == NULL)
    //                break;
    // 
    //            cout<<pNode->Value()<<endl;
    // 
    //            // 遍历二级
    //            TiXmlNode* pChild = pNode->FirstChild();
    //            if (pChild != NULL)
    //            {
    //                cout<<pChild->Value()<<endl;
    //                TiXmlNode* pChild1 = pChild->FirstChild();
    //                while(true)
    //                {
    //                    cout<<pChild1->Value()<<endl;
    //                    pChild1 = pChild->IterateChildren(pChild1);
    //                    if(pChild1 == NULL)
    //                        break;
    //                    cout<<pChild1->Value()<<endl;
    //                }
    //            }
    // 
    //        }      

    }



}

bool GetNodePointerByName(TiXmlElement* pRootEle, const char* strNodeName,TiXmlElement* &destNode)
{
    // if equal root node then return
    if (0 == strcmp(strNodeName, pRootEle->Value()))
    {
        destNode = pRootEle;
        return true;
    }

    TiXmlElement* pEle = pRootEle;  
    for (pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())  
    {  
        // recursive find sub node return node pointer	
        if (0 != strcmp(pEle->Value(), strNodeName))
        {
            if(GetNodePointerByName(pEle,strNodeName,destNode))
                return true;
        }
        else
        {
            destNode = pEle;
            return true;
        }
    }  

    return false;
}
