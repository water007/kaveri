
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/tree.h>

#include "defines.h"

xmlDocPtr doc = NULL;
xmlNodePtr root = NULL;
xmlNodePtr cur = NULL;

int parse_doc(const char *file, const char *root_node){
    doc = xmlParseFile(file);
    if(doc == NULL){
        goto fail;
    }
    root = xmlDocGetRootElement(doc);
    if(root == NULL){
        goto fail;
    }
    if(xmlStrcmp(root->name, root_node)){
        goto fail;
    }
    return 0;
fail:
    xmlFreeDoc(doc);
    return -1;
}

int update_node(const char *cmd, const char *sub, const char *attr, char *value, int rw, char *type){
    if(root == NULL) parse_doc(CONFIG_FILE_PATH, "configs");
    cur = root->children;
    while(cur != NULL){
        if(!xmlStrcmp(cur->name, cmd)){
            cur = cur->children;
            while(cur != NULL){
                if(!xmlStrcmp(cur->name, sub)){
                    if(!rw){
                        value = (char *)xmlGetProp(cur, (const xmlChar *)attr);
                        printf("Test = %s\n", value);
                    }else{
                        printf("Setting\n");
                        xmlAttrPtr ptr = xmlSetProp(cur, attr, (const xmlChar*)value);
                        printf("After setting\n");
                        break;
                    }
                }
                cur = cur->next;   
            }
        }
        cur = cur->next;
    }
    xmlSaveFileEnc(CONFIG_FILE_PATH, doc, "UTF-8");
}