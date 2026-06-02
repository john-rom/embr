#include <errno.h>
#include <string.h>
#include <zephyr/ztest.h>

#include "embr_coap_client.h"
#include "openthread_coap_wrap_mock.h"

static void embr_coap_client_before(void *fixture) {
  (void)fixture;
  openthread_coap_wrap_mock_reset();
}

ZTEST_SUITE(embr_coap_client, NULL, NULL, embr_coap_client_before, NULL, NULL);

ZTEST(embr_coap_client, test_init_success) {
  mock_openthread_coap_wrap_init_return_value = 0;

  int ret = embr_coap_client_init();
  zassert_equal(ret, 0, "embr_coap_client_init should return 0");
  zassert_equal(mock_openthread_coap_wrap_init_call_count, 1,
                "openthread_coap_wrap_init should be called once");
}

ZTEST(embr_coap_client, test_init_fail) {
  mock_openthread_coap_wrap_init_return_value = -EIO;

  int ret = embr_coap_client_init();
  zassert_equal(ret, -EIO, "embr_coap_client_init should return -EIO");
  zassert_equal(mock_openthread_coap_wrap_init_call_count, 1,
                "openthread_coap_wrap_init should be called once");
}

static void assert_send_command_success(enum embr_coap_light_command command) {
  int ret = embr_coap_client_send(command);
  zassert_equal(ret, 0, "embr_coap_client_send should return 0");
  zassert_equal(mock_openthread_coap_wrap_send_put_call_count, 1,
                "openthread_coap_wrap_send_put should be called once");
  zassert_equal(mock_openthread_coap_wrap_send_put_last_port, EMBR_COAP_PORT,
                "destination port should match");
  zassert_not_null(mock_openthread_coap_wrap_send_put_last_uri_path_options,
                   "URI path options should not be NULL");
  zassert_equal(
      strcmp(mock_openthread_coap_wrap_send_put_last_uri_path_options[0],
             EMBR_COAP_LIGHT_URI_PATH),
      0, "URI path should match");
  zassert_is_null(
      mock_openthread_coap_wrap_send_put_last_uri_path_options[1],
      "URI path options should be NULL-terminated");
  zassert_equal(mock_openthread_coap_wrap_send_put_last_payload, command,
                "payload should match command");
  zassert_equal(mock_openthread_coap_wrap_send_put_last_payload_size, 1,
                "payload size should be one byte");
}

ZTEST(embr_coap_client, test_send_command_glow_success) {
  assert_send_command_success(EMBR_COAP_LIGHT_GLOW);
}

ZTEST(embr_coap_client, test_send_command_flip_success) {
  assert_send_command_success(EMBR_COAP_LIGHT_FLIP);
}

ZTEST(embr_coap_client, test_send_command_sleep_success) {
  assert_send_command_success(EMBR_COAP_LIGHT_SLEEP);
}

ZTEST(embr_coap_client, test_send_command_invalid_fail) {
  int ret = embr_coap_client_send((enum embr_coap_light_command)-1);
  zassert_equal(ret, -EINVAL, "embr_coap_client_send should return -EINVAL");
  zassert_equal(mock_openthread_coap_wrap_send_put_call_count, 0,
                "openthread_coap_wrap_send_put should not be called");
}

ZTEST(embr_coap_client,
      test_send_command_nonzero_backend_success_returns_zero_success) {
  mock_openthread_coap_wrap_send_put_return_value = 8;

  int ret = embr_coap_client_send(EMBR_COAP_LIGHT_GLOW);
  zassert_equal(ret, 0, "embr_coap_client_send should normalize success to 0");
  zassert_equal(mock_openthread_coap_wrap_send_put_call_count, 1,
                "openthread_coap_wrap_send_put should be called once");
}

ZTEST(embr_coap_client, test_send_command_backend_error_fail) {
  mock_openthread_coap_wrap_send_put_return_value = -EIO;

  int ret = embr_coap_client_send(EMBR_COAP_LIGHT_GLOW);
  zassert_equal(ret, -EIO, "embr_coap_client_send should return -EIO");
  zassert_equal(mock_openthread_coap_wrap_send_put_call_count, 1,
                "openthread_coap_wrap_send_put should be called once");
}
