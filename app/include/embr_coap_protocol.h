#ifndef EMBR_COAP_PROTOCOL_H_
#define EMBR_COAP_PROTOCOL_H_

/**
 * @file embr_coap_protocol.h
 * @brief Shared CoAP wire contract for embr light commands.
 *
 * Defines the URI, port, and single-byte ASCII command payloads used between
 * embr CoAP clients and server nodes.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** UDP port used for embr CoAP requests. */
#define EMBR_COAP_PORT 5683

/** URI path segment for embr light commands. */
#define EMBR_COAP_LIGHT_URI_PATH "light"

/** Single-byte ASCII payloads accepted by the embr light endpoint. */
enum embr_coap_light_command {
  /** Request that lights turn off. */
  EMBR_COAP_LIGHT_SLEEP = '0',
  /** Request that lights turn on. */
  EMBR_COAP_LIGHT_GLOW = '1',
  /** Request that lights toggle state. */
  EMBR_COAP_LIGHT_FLIP = '2'
};

#ifdef __cplusplus
}
#endif

#endif /* EMBR_COAP_PROTOCOL_H_ */
