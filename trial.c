#include <cups/cups.h>
#include <stdio.h>

typedef struct
{
  int num_dests;
  cups_dest_t *dests;
} my_user_data_t;

int
my_dest_cb(my_user_data_t *user_data, unsigned flags,
           cups_dest_t *dest)
{
  if (flags & CUPS_DEST_FLAGS_REMOVED)
  {
    user_data->num_dests =
        cupsRemoveDest(dest->name, dest->instance,
                       user_data->num_dests,
                       &(user_data->dests));
  }
  else
  {
    user_data->num_dests =
        cupsCopyDest(dest, user_data->num_dests,
                     &(user_data->dests));
  }

  return (1);
}

int
my_get_dests(cups_ptype_t type, cups_ptype_t mask,
             cups_dest_t **dests)
{
  my_user_data_t user_data = { 0, NULL };

  if (!cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, type,
                     mask, (cups_dest_cb_t)my_dest_cb,
                     &user_data))
  {
    cupsFreeDests(user_data.num_dests, user_data.dests);

    *dests = NULL;

    return (0);
  }


  *dests = user_data.dests;

  return (user_data.num_dests);
}

void main(){
    my_get_dests(CUPS_PRINTER_LOCAL,CUPS_PRINTER_DISCOVERED,is_default);
}