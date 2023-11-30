import { App } from '../client/App';
import { renderToString } from 'react-dom/server';

const app = renderToString(App);
const html = `
	<html lang='en'>
	<head>
		<script src='/webassembly/object_detector_on_image_cpp.js' async defer type="text/javascript"></script>
		<script src='/client.js' async defer type="text/javascript"></script>
		<style>
			input {
				margin: 5px;
			}
			.result-container {
				position: relative;
			}

			* {
				font-family: Courier, "Lucida Console", monospace;
			}
		</style>
	</head>
	<body>
		<div id='root'>${app}</div>
	</body>
	</html>
`;

export const htmlString = html;
