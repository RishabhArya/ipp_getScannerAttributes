#include <cups/cups.h>
#include <stdio.h>

ipp_t *response;

void generateRequest()
{

http_t *httpHandler;
httpHandler = httpConnect2("http://1.2.3.4:5678/ipp/scan", 631, NULL, AF_UNSPEC, HTTP_ENCRYPTION_IF_REQUESTED, 1, 50000, NULL);
 
//Creating a request
ipp_t *newRequesthandler;
ipp_op_t operation_code = IPP_OP_GET_PRINTER_ATTRIBUTES;
newRequesthandler = ippNewRequest(operation_code);

//Adding Attributes
const char *uri = "ipp://1.2.3.4:5678/ipp/scan";
//const char *name = "ipp";

static const char * const requested_attributes[] =
{
  "printer-description",
  "job-template",
  "media-col-database"
};

ippAddString(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, uri);
ippAddString(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", NULL, cupsUser());
//ippAddString(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_MIMETYPE, "document-format", NULL, "image/pwg-raster");
ippAddStrings(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_KEYWORD, "requested-attributes", (int)(sizeof(requested_attributes) / sizeof(requested_attributes[0])), NULL, requested_attributes);

response = cupsDoRequest(httpHandler, newRequesthandler, uri);
httpClose(httpHandler);
}

//To parse a response:
void parseResponse(){
  
  // Declare the file pointer 
  FILE *filePointer ; 
  filePointer = fopen("ScannerAttributes.txt", "w") ; 

  ipp_attribute_t *attr;
  const char *name;
  char value[2048];
  printf("<---Printing Printers Attributes--->\n");
  for (attr = ippFirstAttribute(response); attr; attr = ippNextAttribute(response))
  {
    name = ippGetName(attr);
    if (name)
    {
      ippAttributeString(attr, value, sizeof(value));
      printf("%s => %s\n", name, value);
      fputs(name, filePointer);
      fputs(value, filePointer);
      fputs("\n", filePointer);
    }
  }
  fclose(filePointer);
}

void main()
{
  generateRequest();
  parseResponse();
}
