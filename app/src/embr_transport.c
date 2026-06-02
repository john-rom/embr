#include <errno.h>

#include "embr_coap_client.h"
#include "embr_transport.h"

static int send_coap_command(embr_command command) {
  switch (command) {
  case EMBR_GLOW:
    return embr_coap_client_send(EMBR_COAP_LIGHT_GLOW);

  case EMBR_FLIP:
    return embr_coap_client_send(EMBR_COAP_LIGHT_FLIP);

  case EMBR_SLEEP:
    return embr_coap_client_send(EMBR_COAP_LIGHT_SLEEP);

  default:
    return -EINVAL;
  }
}

int embr_transport_init(void) {
  return embr_coap_client_init();
}

int embr_transport_send_command(embr_command command) {
  int err = 0;

  switch (command) {
  case EMBR_GLOW:
  case EMBR_FLIP:
  case EMBR_SLEEP:
    err = send_coap_command(command);
    if (err) {
      return err;
    }
    break;

  case UNKNOWN:
  case NOISE:
    return 0;

  case EMBR_RESET:
  default:
    return -EINVAL;
  }

  return 0;
}
