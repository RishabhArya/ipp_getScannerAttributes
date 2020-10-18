#include <cups/cups.h>
#include <stdio.h>

typedef struct _ippdata_t
{
  size_t	rpos,			/* Read position */
		wused,			/* Bytes used */
		wsize;			/* Max size of buffer */
  ipp_uchar_t	*wbuffer;		/* Buffer */
} _ippdata_t;

ssize_t write_cb(_ippdata_t *data, ipp_uchar_t *buffer, size_t bytes)
{
  size_t	count;


  if ((count = data->wsize - data->wused) > bytes)
    count = bytes;

  memcpy(data->wbuffer + data->wused, buffer, count);
  data->wused += count;

  return ((ssize_t)count);
}

ssize_t read_cb(_ippdata_t *data, ipp_uchar_t *buffer, size_t bytes)
{
  size_t	count;	

  if ((count = data->wsize - data->rpos) > bytes)
    count = bytes;

  memcpy(buffer, data->wbuffer + data->rpos, count);
  data->rpos += count;

  return ((ssize_t)count);
}

_ippdata_t	data;		/* IPP buffer */

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
const char *charset = "utf-8";
const char *language = "en-US";
ipp_uchar_t	buffer[8192];	/* Write buffer data */

static const char * const requested_attrs[] =
{	
    "job-template",
    "printer-description"
};

ippAddString(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, uri);
ippAddString(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_CHARSET, "attributes-charset", NULL, charset);
ippAddString(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_LANGUAGE, "attributes-natural-language", NULL, "en-US");
//ippAddStrings(newRequesthandler, IPP_TAG_OPERATION, IPP_TAG_KEYWORD, "requested-attributes", (int)(sizeof(requested_attrs) / sizeof(requested_attrs[0])), NULL, requested_attrs);

//Writing ippWriteIO
data.wused   = 0;
data.wsize   = sizeof(buffer);
data.wbuffer = buffer;

ippWriteIO(&data, (ipp_iocb_t)write_cb, 1, NULL, newRequesthandler);

return cupsDoFileRequest( httpHandler, newRequesthandler, "/ipp/request", "testfile.txt");

}

//To parse a response:
void parseResponse(ipp_t *cupsFileresponse){
    ipp_t *newResponsehandler;
    
    newResponsehandler = ippNew();
    newResponsehandler = cupsFileresponse;
    data.rpos = 0;
    ippReadIO( &data, (ipp_iocb_t)read_cb, 1, NULL, newResponsehandler);

    ipp_attribute_t *attr;
    const char *name;
    char value[2048];

    for (attr = ippFirstAttribute(newResponsehandler); attr; attr = ippNextAttribute(newResponsehandler))
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
