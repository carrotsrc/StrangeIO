#ifndef __RACKDESC_H_1439063050__
#define __RACKDESC_H_1439063050__

#include "framework/common.h"
namespace StrangeIO {
namespace Config {

typedef struct {

	typedef struct {

		typedef struct {
			int num_workers;
		} Threading;
		Threading threads;

	} System;

	typedef struct {

		typedef struct {
			std::string label;
			std::string port;
		} Controller;

		std::vector<Controller> controllers;

	} Midi;

	typedef struct {

		typedef struct {
			std::string from, plug, to, jack;
		} Link;

		typedef struct {

			typedef struct {
				std::string type, value;
			} Config;

			typedef struct {
				std::string name, module;
				int code;
			} Binding;

			std::string label, unit, library;
			std::vector<Config> configs;
			std::vector<Binding> bindings;
		} Unit;

		std::vector<std::string> mainlines;
		std::vector<Link> daisychains;
		std::vector<Unit> units;

	} Setup;

	System system;
	Midi midi;
	Setup setup;


} RackDesc;



} // Config
} // StrangeIO
#endif
