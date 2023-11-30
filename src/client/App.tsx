import React, { useEffect, useMemo, useRef, useState } from 'react';
import { AppCoordinate, processImages } from '../../webassembly/src/utils';

const AppBase: React.FC = () => {
	const imgRef = useRef<HTMLImageElement | null>(null);
	const [img, setImg] = useState<File>();
	const [templ, setTempl] = useState<File>();
	const [data, setData] = useState<AppCoordinate>();

	const setImgFile = async (files: FileList|null, setState: Function) => { 
		const file = files ? files[0] : null;
		setState(file);
	}

	useEffect(() => {
		if (!img || !templ) {
			return;
		}
		(async () => {
			const result = await processImages(img, templ);
			result && setData(result);
		})();
	}, [img, templ])

	const imgUrl = useMemo(() => {
		return img ? URL.createObjectURL(img) : '';
	}, [img]);

	return (
		<div>
			<h1>Find image</h1>
			<label>
				Image base
				<input type="file" accept=".png, .jpg, .jpeg" onChange={(e) => setImgFile(e.target.files, setImg)} />
			</label>
			<label>
				Image to find
				<input type="file" accept=".png, .jpg, .jpeg" onChange={(e) => setImgFile(e.target.files, setTempl)} />
			</label>
			<div className="result-container">
				<img ref={imgRef} src={imgUrl} />
				<span style={{
					border: 'solid 3px red',
					display: data?.x ? 'block' : 'none',
					position: 'absolute',
					width:  `${data?.width || 0}px`,
					height: `${data?.height || 0}px`,
					top: `${data?.y || 0}px`,
					left: `${data?.x || 0}px`
					}}
				/>
			</div>
		</div>
	);
};

export const App = <AppBase />;
