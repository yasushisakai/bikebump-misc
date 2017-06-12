import { createReadStream, createWriteStream, readdir } from 'fs'

const dir: string = '/Users/yasushisakai/Desktop/testRings/'

function readdirPromise (directory: string): Promise<string []> {
  return new Promise((resolve, reject) => {
    readdir(directory, (error, files: string[]) => {
      if (error !== null) {
        return reject(error)
      }
      resolve(files.filter(excludeHidden))
    })
  })
}

function excludeHidden (dirname: string): boolean {
  return !(dirname.startsWith('.') || dirname === 'random')
}

function bellFrequencyMap (bellName: string): number {
  switch (bellName) {
    case 'gold' :
      return 2484
    case 'oi' :
      return 2765
    case 'silver' :
      return 2390
    case 'hubway' :
      return 2953
    case 'crane' :
      return 3609
    default :
      return 3000
  }
}

function folderCodeMap (folder: string): string {
  switch (folder) {
    case 'noisy-single' :
      return 'n_1_0'
    case 'noisy-double' :
      return 'n_0_1'
    case 'silent-single' :
      return 's_1_0'
    case 'silent-double' :
      return 's_0_1'
    default :
      return 'n_0_0'
  }
}

async function main () {
  const ringFolders: string[] = await readdirPromise(dir)
  const randomFileNames = await readdirPromise(`${dir}/random`)
  ringFolders.map( async (folders: string) => {
    const fullPath = `${dir}${folders}`
    const typeFolders = await readdirPromise(fullPath)

    typeFolders.map( async (folderName: string) => {
      const fullPathToFolder = `${fullPath}/${folderName}`
      randomFileNames.map((filename: string) => {
        const id: string = filename.split('_')[1].split('.')[0]
        const newRandomFilePath: string = `${dir}random_${id}_n_0_0_${bellFrequencyMap(folders)}.wav`
        // console.log(newRandomFilePath)
        createReadStream(`${dir}/random/${filename}`).pipe(createWriteStream(newRandomFilePath))
      })

      const filenames = await readdirPromise(fullPathToFolder)

      filenames.map ( (filename: string) => {
        // console.log(`${fullPathToFolder}/${filename}`)
        const id: string = filename.split('_')[1].split('.')[0]
        const code: string = folderCodeMap(folderName)
        const freq: number = bellFrequencyMap(folders)
        const newFileName: string = `${dir}${folders}_${id}_${code}_${freq}.wav`
        createReadStream(`${fullPathToFolder}/${filename}`).pipe(createWriteStream(newFileName))
      })
    })

  })
}

main()
