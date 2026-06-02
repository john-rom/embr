#include <errno.h>
#include <zephyr/ztest.h>

#include "embr_coap_client_mock.h"
#include "embr_transport.h"
#include "embr_types.h"

static void embr_transport_before(void *fixture) {
  (void)fixture;
  embr_coap_client_mock_reset();
}

ZTEST_SUITE(embr_transport, NULL, NULL, embr_transport_before, NULL, NULL);

ZTEST(embr_transport, test_init_success) {
  mock_embr_coap_client_init_return_value = 0;

  int ret = embr_transport_init();
  zassert_equal(ret, 0, "embr_transport_init should return 0");
  zassert_equal(mock_embr_coap_client_init_call_count, 1,
                "embr_coap_client_init should be called once");
}

ZTEST(embr_transport, test_init_fail) {
  mock_embr_coap_client_init_return_value = -EIO;

  int ret = embr_transport_init();
  zassert_equal(ret, -EIO, "embr_transport_init should return -EIO");
  zassert_equal(mock_embr_coap_client_init_call_count, 1,
                "embr_coap_client_init should be called once");
}

ZTEST(embr_transport, test_send_command_glow_success) {
  int ret = embr_transport_send_command(EMBR_GLOW);
  zassert_equal(ret, 0, "embr_transport_send_command should return 0");
  zassert_equal(mock_embr_coap_client_send_call_count, 1,
                "embr_coap_client_send should be called once");
  zassert_equal(mock_embr_coap_client_send_last_command, EMBR_COAP_LIGHT_GLOW,
                "embr_coap_client_send should receive EMBR_COAP_LIGHT_GLOW");
}

ZTEST(embr_transport, test_send_command_flip_success) {
  int ret = embr_transport_send_command(EMBR_FLIP);
  zassert_equal(ret, 0, "embr_transport_send_command should return 0");
  zassert_equal(mock_embr_coap_client_send_call_count, 1,
                "embr_coap_client_send should be called once");
  zassert_equal(mock_embr_coap_client_send_last_command, EMBR_COAP_LIGHT_FLIP,
                "embr_coap_client_send should receive EMBR_COAP_LIGHT_FLIP");
}

ZTEST(embr_transport, test_send_command_sleep_success) {
  int ret = embr_transport_send_command(EMBR_SLEEP);
  zassert_equal(ret, 0, "embr_transport_send_command should return 0");
  zassert_equal(mock_embr_coap_client_send_call_count, 1,
                "embr_coap_client_send should be called once");
  zassert_equal(mock_embr_coap_client_send_last_command, EMBR_COAP_LIGHT_SLEEP,
                "embr_coap_client_send should receive EMBR_COAP_LIGHT_SLEEP");
}

ZTEST(embr_transport, test_send_command_unknown_ignored_success) {
  int ret = embr_transport_send_command(UNKNOWN);
  zassert_equal(ret, 0, "embr_transport_send_command should return 0");
  zassert_equal(mock_embr_coap_client_send_call_count, 0,
                "embr_coap_client_send should not be called");
}

ZTEST(embr_transport, test_send_command_noise_ignored_success) {
  int ret = embr_transport_send_command(NOISE);
  zassert_equal(ret, 0, "embr_transport_send_command should return 0");
  zassert_equal(mock_embr_coap_client_send_call_count, 0,
                "embr_coap_client_send should not be called");
}

ZTEST(embr_transport, test_send_command_reset_fail) {
  int ret = embr_transport_send_command(EMBR_RESET);
  zassert_equal(ret, -EINVAL,
                "embr_transport_send_command should return -EINVAL");
  zassert_equal(mock_embr_coap_client_send_call_count, 0,
                "embr_coap_client_send should not be called");
}

ZTEST(embr_transport, test_send_command_invalid_fail) {
  int ret = embr_transport_send_command((embr_command)-1);
  zassert_equal(ret, -EINVAL,
                "embr_transport_send_command should return -EINVAL");
  zassert_equal(mock_embr_coap_client_send_call_count, 0,
                "embr_coap_client_send should not be called");
}

ZTEST(embr_transport, test_send_command_backend_error_fail) {
  mock_embr_coap_client_send_return_value = -EIO;

  int ret = embr_transport_send_command(EMBR_GLOW);
  zassert_equal(ret, -EIO, "embr_transport_send_command should return -EIO");
  zassert_equal(mock_embr_coap_client_send_call_count, 1,
                "embr_coap_client_send should be called once");
}
