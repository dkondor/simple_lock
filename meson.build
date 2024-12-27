project(
	'simple_lock',
	'c',
	version: '0.0.1',
	license: 'GPLv3+',
	meson_version: '>=0.58.0',
	default_options: [
        'c_std=c11',
	],
)

# wayland-scanner -- needed for using the ext-session-lock protocol
wayland_client = dependency('wayland-client', version: '>=1.1.91')
wayland_scanner = find_program('wayland-scanner')
wayland_protos = dependency('wayland-protocols', version: '>=1.25')
wl_protocol_dir = wayland_protos.get_variable('pkgdatadir')

wayland_scanner_code = generator(
	wayland_scanner,
	output: '@BASENAME@-protocol.c',
	arguments: ['private-code', '@INPUT@', '@OUTPUT@'],
)

wayland_scanner_client = generator(
	wayland_scanner,
	output: '@BASENAME@-client-protocol.h',
	arguments: ['client-header', '@INPUT@', '@OUTPUT@'],
)

client_protocols = [
    wl_protocol_dir + '/staging/ext-session-lock/ext-session-lock-v1.xml'
]

wl_protos_client_src = []
wl_protos_headers = []

foreach p : client_protocols
	xml = join_paths(p)
	wl_protos_headers += wayland_scanner_client.process(xml)
	wl_protos_client_src += wayland_scanner_code.process(xml)
endforeach

lib_lock_protos = static_library('lock_protos', wl_protos_client_src + wl_protos_headers,
	dependencies: [wayland_client]) # for the include directory

protos = declare_dependency(
	link_with: lib_lock_protos,
	sources: wl_protos_headers,
)

simple_lock = executable('simple_lock', ['simple_lock.c', 'main.c'],
	dependencies: [wayland_client, protos], install: false)


