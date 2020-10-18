#include <cups/cups.h>
#include <stdio.h>

// Generate Request
ipp_t * generateRequest(){

http_t *httpHandler;
httpHandler = httpConnect2("ipp", 631, NULL, AF_UNSPEC, HTTP_ENCRYPTION_IF_REQUESTED, 1, 50000, NULL);
 
//Creating a request
ipp_t *newRequesthandler;
ipp_op_t operation_code = IPP_OP_GET_PRINTER_ATTRIBUTES;
newRequesthandler = ippNewRequest(operation_code);

//Adding Attributes
const char *uri = "http://1.2.3.4:5678/ipp/scan";
const char *name = "rishabh arya";
const char *language = "en-US";

static const char * const requested_attributes[] =
{
  "printer-description",
  "job-template",
  "media-col-database"
};

ippAddString(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, uri);
ippAddString(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", NULL, name);
ippAddString(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_MIMETYPE, "document-format", NULL, "image/pwg-raster");
ippAddStrings(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_KEYWORD, "requested-attributes", (int)(sizeof(requested_attributes) / sizeof(requested_attributes[0])), NULL, requested_attributes);

return cupsDoRequest(httpHandler, newRequesthandler, "/ipp/scan");;
}

//To parse a response:
void parseResponse(ipp_t *response){
  ipp_attribute_t *attr;
  const char *name;
  char value[2048];

  for (attr = ippFirstAttribute(response); attr; attr = ippNextAttribute(response))
  {
    name = ippGetName(attr);
    if (name)
    {
      ippAttributeString(attr, value, sizeof(value));
      printf("%s=%s\n", name, value);
    }
  }
}

void main()
{
    parseResponse(generateRequest());
}
