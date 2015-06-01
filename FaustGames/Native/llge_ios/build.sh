xcodebuild -project llge_ios.xcodeproj -target llge_ios -sdk iphonesimulator -configuration Release clean build
cp build/Release-iphonesimulator/libllge_ios.a libllge_ios-i386.a
xcodebuild -project llge_ios.xcodeproj -target llge_ios -sdk iphoneos -arch arm64 -configuration Release clean build
cp build/Release-iphoneos/libllge_ios.a libllge_ios-arm64.a
xcodebuild -project llge_ios.xcodeproj -target llge_ios -sdk iphoneos -arch armv7 -configuration Release clean build
cp build/Release-iphoneos/libllge_ios.a libllge_ios-armv7.a
lipo -create -output libllge_ios.a libllge_ios-i386.a libllge_ios-arm64.a libllge_ios-armv7.a


