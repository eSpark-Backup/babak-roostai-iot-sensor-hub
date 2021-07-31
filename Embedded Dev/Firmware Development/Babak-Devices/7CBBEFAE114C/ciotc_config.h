/******************************************************************************
 * Copyright 2018 Google
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
// This file contains your configuration used to connect to Cloud IoT Core

const char *ssid = "TP-LINK_3D1D";
const char *password = "YUEP6YMEER";

//const char *ssid = "eSpark Consulting Group";
//const char *password = "eSparkCons";

//const char *ssid = "Siblings";
//const char *password = "clashofclans2";

// Cloud IoT details.
const char *project_id = "thermostat-289211";
const char *location = "us-central1";
const char *registry_id = "Reg_R3";
const char *device_id = "esp32";
// Configuration for NTP
const char* ntp_primary = "pool.ntp.org";
const char* ntp_secondary = "time.nist.gov";

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

// To get the private key run (where private-key.pem is the ec private key
// used to create the certificate uploaded to google cloud iot):
// openssl ec -in <private-key.pem> -noout -text
// and copy priv: part.
// The key length should be exactly the same as the key length bellow (32 pairs
// of hex digits). If it's bigger and it starts with "00:" delete the "00:". If
// it's smaller add "00:" to the start. If it's too big or too small something
// is probably wrong with your key.
const char *private_key_str =
"23:e1:1d:c8:c9:d8:82:d7:05:e8:58:fe:bd:58:a1:"
"b6:c7:d7:87:97:fd:3d:13:14:0f:f1:87:f1:d5:85:"
"22:a7";
;

// Time (seconds) to expire token += 20 minutes for drift
const int jwt_exp_secs = 60*20; // Maximum 24H (3600*24)

// To get the certificate for your region run:
//   openssl s_client -showcerts -connect mqtt.googleapis.com:8883
// for standard mqtt or for LTS:
//   openssl s_client -showcerts -connect mqtt.2030.ltsapis.goog:8883
// Copy the certificate (all lines between and including ---BEGIN CERTIFICATE---
// and --END CERTIFICATE--) to root.cert and put here on the root_cert variable.

const char *root_cert =
"-----BEGIN CERTIFICATE-----\n"
"MIIDDDCCArKgAwIBAgIURjzskJE39lD3SX0IkTs5jt4noWYwCgYIKoZIzj0EAwIw\n"
"RDELMAkGA1UEBhMCVVMxIjAgBgNVBAoTGUdvb2dsZSBUcnVzdCBTZXJ2aWNlcyBM\n"
"TEMxETAPBgNVBAMTCEdUUyBMVFNYMB4XDTIwMDcxMzAwMDAwMFoXDTIxMDcxMjAw\n"
"MDAwMFowbTELMAkGA1UEBhMCVVMxEzARBgNVBAgMCkNhbGlmb3JuaWExFjAUBgNV\n"
"BAcMDU1vdW50YWluIFZpZXcxEzARBgNVBAoMCkdvb2dsZSBMTEMxHDAaBgNVBAMM\n"
"EyouMjAzMC5sdHNhcGlzLmdvb2cwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARh\n"
"fb8NcIBEcwbwSJKKlnjJ7cDwKMHiHeQd1xgE4PWzqEFxauPG30WniyoFZQDtgqft\n"
"Tq6iImzs1iP4cR5xN9Hao4IBVzCCAVMwEwYDVR0lBAwwCgYIKwYBBQUHAwEwDgYD\n"
"VR0PAQH/BAQDAgeAMB4GA1UdEQQXMBWCEyouMjAzMC5sdHNhcGlzLmdvb2cwDAYD\n"
"VR0TAQH/BAIwADAfBgNVHSMEGDAWgBSzK6ugSBx+E4rJCMRAQiKiNlHiCjBpBggr\n"
"BgEFBQcBAQRdMFswLwYIKwYBBQUHMAKGI2h0dHA6Ly9wa2kuZ29vZy9ndHNsdHNy\n"
"L2d0c2x0c3guY3J0MCgGCCsGAQUFBzABhhxodHRwOi8vb2NzcC5wa2kuZ29vZy9H\n"
"VFNMVFNYMCEGA1UdIAQaMBgwDAYKKwYBBAHWeQIFAzAIBgZngQwBAgIwMAYDVR0f\n"
"BCkwJzAloCOgIYYfaHR0cDovL2NybC5wa2kuZ29vZy9HVFNMVFNYLmNybDAdBgNV\n"
"HQ4EFgQU3VXinuGqY4eo6dBUviU9iLPkWTcwCgYIKoZIzj0EAwIDSAAwRQIhAO4t\n"
"PT9rr/pZxYx3KSzXRK0l0bxpVELE2WS43FIYvWYzAiBwEHAunJZ5oBXfPXH6sJkr\n"
"Xt+ea9ZwVMYLXWQnGvKQ+Q==\n"
"-----END CERTIFICATE-----\n";

// In case we ever need extra topics
//const int ex_num_topics = 0;
//const char* ex_topics[ex_num_topics];
const int ex_num_topics = 1;
const char* ex_topics[ex_num_topics] = {
  "/projects/thermostat-289211/topics/Topic_R3"
};
