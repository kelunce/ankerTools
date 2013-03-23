#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <msxml2.h>
#include <comdef.h>


#pragma comment(lib,"msxml2")

using namespace std;

int main()
{
    // 示例XML
    char  pBuff[2048] ={0};
    strcat(pBuff, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
    strcat(pBuff, "<!-- edited with XMLSpy v2010 rel. 3 sp1 (http://www.altova.com) by MESMERiZE (MSM) -->");
    strcat(pBuff, "    <xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" elementFormDefault=\"qualified\" attributeFormDefault=\"unqualified\">");
    strcat(pBuff, "        <xs:annotation>");
    strcat(pBuff, "            <xs:documentation>通知消息</xs:documentation>");
    strcat(pBuff, "        </xs:annotation>");
    strcat(pBuff, "        <xs:complexType name=\"ConsumeResultPost\">");
    strcat(pBuff, "            <xs:element name=\"userId\" value=\"20132222\"/>");
    strcat(pBuff, "            <xs:element name=\"contentId\" value=\"cid\"/>");
    strcat(pBuff, "            <xs:element name=\"consumeCode\" value=\"cc6666\"/>");
    strcat(pBuff, "            <xs:element name=\"cpid\" value=\"cpid0\"/>");
    strcat(pBuff, "            <xs:element name=\"hRet\" value=\"2\"/>");
    strcat(pBuff, "            <xs:element name=\"versionId\" value=\"2_0_0\"/>");
    strcat(pBuff, "            <xs:element name=\"cpparam\" value=\"cpppp\"/>");
    strcat(pBuff, "            <xs:element name=\"packageID\" value=\"29\"/>");
    strcat(pBuff, "         </xs:complexType>");
    strcat(pBuff, "    </xs:schema>");

    // 创建解析器
    IXMLDOMDocument*	pXMLDoc = NULL;
    CoInitialize(NULL);
    CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_ALL, IID_IXMLDOMDocument, (void**)&pXMLDoc);
    
    // 加载解析XML 
    _bstr_t bStr = pBuff;
    VARIANT_BOOL vBSucc;
    pXMLDoc->loadXML(bStr,&vBSucc);

    //BSTR text;
    pXMLDoc->get_xml(/*&text*/bStr.GetAddress());

    bStr = "xs:schema";
    IXMLDOMNodeList *pNodeList = NULL;
    pXMLDoc->selectNodes(bStr,&pNodeList);
    //pXMLDoc->getElementsByTagName();

    long lLen = 0;
    pNodeList->get_length(&lLen);

    char szStr[128];
    ZeroMemory(szStr,sizeof(szStr));
    IXMLDOMNode *pNode;
    pNodeList->get_item( 0, &pNode );

    // GetElementText
    BSTR			bstr = NULL;
    pNode->get_text( &bstr );
    strcpy( szStr,(const char*)_bstr_t(bstr,FALSE) );


    // GetAttributeText

    wchar_t					wstrAttr[128];
    IXMLDOMNode*			pAttrNode = NULL;
    IXMLDOMNamedNodeMap*	pMap = NULL;
    VARIANT					varValue;    
    
    int n = mbstowcs( wstrAttr, "attributeFormDefault", 128 );
    pNode->get_attributes( &pMap );
    pMap->getNamedItem( wstrAttr, &pAttrNode );
    pAttrNode->get_nodeValue( &varValue );
    strcpy( szStr,(const char*)_bstr_t(varValue.bstrVal,FALSE) );

    if(pAttrNode != NULL)
        pAttrNode->Release();

    if(pMap != NULL)
        pMap->Release();

    cout<<(char*)bStr<<endl;


    CoUninitialize();
    return 0;
}