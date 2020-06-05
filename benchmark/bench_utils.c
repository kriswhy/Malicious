/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bench_utils.h"

#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

static bool parse_long(long* value, const char* arg) {
  errno        = 0;
  const long v = strtol(arg, NULL, 10);

  if ((errno == ERANGE && (v == LONG_MAX || v == LONG_MIN)) || (errno != 0 && v == 0)) {
    return false;
  }
  *value = v;

  return true;
}

static bool parse_uint32_t(uint32_t* value, const char* arg) {
  long tmp = 0;
  if (!parse_long(&tmp, arg)) {
    return false;
  }

  if (tmp < 0 || (unsigned long)tmp > UINT32_MAX) {
    return false;
  }

  *value = tmp;
  return true;
}

static void print_usage(const char* arg0) {
  printf("usage: %s [-i iterations] instance\n", arg0);
}

bool parse_args(bench_options_t* options, int argc, char** argv) {
  if (argc <= 1) {
    print_usage(argv[0]);
    return false;
  }

  options->params = 0;
  options->iter   = 10;

  static const struct option long_options[] = {
    {"iter", required_argument, NULL, 'i'},
    {0, 0, 0, 0}
  };

  int c            = -1;
  int option_index = 0;

  while ((c = getopt_long(argc, argv, "i:l:", long_options, &option_index)) != -1) {
    switch (c) {
    case 'i':
      if (!parse_uint32_t(&options->iter, optarg)) {
        printf("Failed to parse argument as positive base-10 number!\n");
        return false;
      }
      break;

    case '?':
    default:
      printf("usage: %s [-i iter] param\n", argv[0]);
      return false;
    }
  }

  if (optind == argc - 1) {
    uint32_t p = 0;
    if (!parse_uint32_t(&p, argv[optind])) {
      printf("Failed to parse argument as positive base-10 number!\n");
      return false;
    }

    if (p <= 0 || p >= 7) {
      printf("Invalid parameter set selected!\n");
      return false;
    }
    options->params = p;
  } else {
    print_usage(argv[0]);
    return false;
  }

  return true;
}
